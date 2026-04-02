#!/usr/bin/env python3
"""
Generate arm_robot.xml (MJCF) from the robotic arm URDF.

The mujoco_ros2_control plugin needs the robot defined in the MJCF scene
with explicit position actuators matching the ros2_control joint names.

Usage:
  python3 generate_mjcf.py
"""

import os
import re
import mujoco

WORKSPACE = os.path.expanduser("~/ros2_ws")
INSTALL = os.path.join(WORKSPACE, "install")
URDF_PATH = os.path.join(INSTALL, "robotic_arm_model_v3/share/robotic_arm_model_v3/urdf/robotic_arm_model_v3.urdf")
OUTPUT_PATH = os.path.join(INSTALL, "robotic_arm_v3_config/share/robotic_arm_v3_config/config/mujoco/arm_robot.xml")
SRC_OUTPUT = os.path.join(WORKSPACE, "src/robot/robotic_arm_v3_config/config/mujoco/arm_robot.xml")

MESH_DIR = os.path.join(INSTALL, "robotic_arm_model_v3/share/robotic_arm_model_v3/meshes")

# Controlled joints (have command interfaces in ros2_control)
ACTUATED_JOINTS = ["joint_1", "joint_2", "joint_3", "joint_4", "end_effector_joint"]

def main():
    # Read URDF and resolve package:// URIs to absolute paths
    with open(URDF_PATH) as f:
        urdf = f.read()

    urdf = urdf.replace(
        "package://robotic_arm_model_v3/meshes/",
        MESH_DIR + "/"
    )

    # Inject <mujoco><compiler meshdir=...></mujoco> so MuJoCo can find mesh files.
    # Must be inside <robot ...> tag. The robot tag spans multiple lines so use regex.
    mujoco_compiler = f'\n  <mujoco><compiler meshdir="{MESH_DIR}"/></mujoco>'
    # Insert after closing > of <robot ...> tag
    urdf = re.sub(r'(<robot\s[^>]*>)', r'\1' + mujoco_compiler, urdf, count=1)

    print("Parsing URDF with MuJoCo...")
    try:
        model = mujoco.MjModel.from_xml_string(urdf)
    except Exception as e:
        print(f"ERROR: MuJoCo failed to parse URDF: {e}")
        return

    print(f"  Parsed OK — {model.njnt} joints, {model.nbody} bodies, {model.ngeom} geoms")

    # Save auto-generated MJCF to a temp file
    tmp_path = "/tmp/arm_auto.xml"
    mujoco.mj_saveLastXML(tmp_path, model)
    print(f"  Auto-generated MJCF saved to {tmp_path}")

    # Read the auto-generated XML and inject actuators + compiler settings
    with open(tmp_path) as f:
        xml = f.read()

    # Build actuator block — position actuators with gear=1
    actuator_lines = []
    for jname in ACTUATED_JOINTS:
        actuator_lines.append(
            f'    <position name="{jname}" joint="{jname}" kp="100" kv="10" ctrlrange="-6.28 6.28"/>'
        )
    actuator_block = "<actuator>\n" + "\n".join(actuator_lines) + "\n  </actuator>"

    # Inject actuator block before </mujoco>
    xml = xml.replace("</mujoco>", f"  {actuator_block}\n</mujoco>")

    # Fix compiler assetdir to point to the mesh directory
    if "<compiler" in xml:
        xml = re.sub(r'<compiler([^/]*/?>)', f'<compiler meshdir="{MESH_DIR}" \\1', xml, count=1)
    else:
        xml = xml.replace("<mujoco", f'<mujoco>\n  <compiler meshdir="{MESH_DIR}"/\n<mujoco', count=1)

    # Write to both src/ (for version control) and install/ (for runtime)
    for out_path in [SRC_OUTPUT, OUTPUT_PATH]:
        os.makedirs(os.path.dirname(out_path), exist_ok=True)
        with open(out_path, "w") as f:
            f.write(xml)
        print(f"  Written: {out_path}")

    # Verify it loads back
    print("Verifying generated MJCF loads...")
    try:
        model2 = mujoco.MjModel.from_xml_path(OUTPUT_PATH)
        print(f"  OK — {model2.njnt} joints, {model2.nu} actuators")
        # Print joint and actuator names
        for i in range(model2.njnt):
            jname = mujoco.mj_id2name(model2, mujoco.mjtObj.mjOBJ_JOINT, i)
            print(f"    joint[{i}]: {jname}")
        for i in range(model2.nu):
            aname = mujoco.mj_id2name(model2, mujoco.mjtObj.mjOBJ_ACTUATOR, i)
            print(f"    actuator[{i}]: {aname}")
    except Exception as e:
        print(f"ERROR: Generated MJCF failed to load: {e}")

if __name__ == "__main__":
    main()
