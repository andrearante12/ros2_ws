# Imitation Learning — Cube Lift Training Guide

Train a behavioral cloning policy to autonomously lift a randomly-placed cube in MuJoCo, then deploy it to the real arm via sim-to-real mirroring.

## Overview

```
Human teleop → demo_recorder → .pt episodes → train → policy → MuJoCo → real arm
```

The pipeline has three stages:

1. **Record** — Teleoperate pick-and-lift demos while `demo_recorder` captures observation-action pairs
2. **Train** — Behavioral cloning (MLP) learns to map observations to actions
3. **Deploy** — Trained policy runs autonomously in MuJoCo, optionally mirroring to the real arm

**Observation (13D):** 7 joint positions + 3D end-effector position + 3D cube position
**Action (4D):** delta XYZ movement + gripper open/close
**Training target:** Lift the cube above the table. After a successful lift, a hardcoded sequence places it at a fixed drop zone.

---

## Prerequisites

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --cmake-args "-DCMAKE_EXE_LINKER_FLAGS=-lcurl"
source install/setup.bash
```

Verify PyTorch is available:
```bash
python3 -c "import torch; print(torch.__version__)"
```

---

## Step 1: Record Demonstrations

You need 30–50 successful lift demonstrations. Each takes about 15–30 seconds.

**Terminal 1 — Launch MuJoCo simulation:**
```bash
ros2 launch arm_bringup mujoco_mirror.launch.py dry_run:=true
```

**Terminal 2 — Launch the demo recorder:**
```bash
ros2 run moveit_controls demo_recorder
```

### Recorder Controls

| Key | Action |
|-----|--------|
| R | Reset sim + randomize cube position |
| Enter | Start / stop recording |
| Ctrl+C | Quit |

### Recording Workflow

1. Press **R** to randomize the cube position
2. Press **Enter** to start recording
3. In the teleop terminal (xterm window), teleoperate the arm:
   - Move above the cube (WASD/QE)
   - Lower to the cube
   - Press **C** to close the gripper
   - Lift up (Q)
4. Press **Enter** to stop recording
5. Type **y** if the lift was successful, **n** if it failed
6. Repeat from step 1

Episodes are saved to `~/demonstrations/episodes/episode_XXXX.json`.

### Tips for Good Demonstrations

- **Be consistent.** Similar motions across demos help the policy learn a clean strategy.
- **Keep it simple.** Move to the cube, grasp, lift straight up. Avoid unnecessary detours.
- **Mark failures honestly.** Failed grasps are filtered out during training.
- **Vary your approach slightly.** Small natural variation helps generalization, but don't be erratic.
- **20 demos** is enough for initial testing. **50 demos** gives solid results.

### Verify Recorded Data

```bash
ls ~/demonstrations/episodes/
python3 -c "
import json, os
d = os.path.expanduser('~/demonstrations/episodes')
files = sorted(f for f in os.listdir(d) if f.endswith('.json'))
print(f'{len(files)} episodes')
for f in files[:3]:
    with open(os.path.join(d, f)) as fh:
        ep = json.load(fh)
    T = len(ep['observations'])
    print(f'  {f}: {T} steps, success={ep[\"success\"]}')
"
```

---

## Step 2: Train the Policy

```bash
python3 -m moveit_controls.il.train
```

### Training Options

```bash
# Custom episode directory
python3 -m moveit_controls.il.train --episodes_dir ~/demonstrations/episodes

# More epochs or different learning rate
python3 -m moveit_controls.il.train --epochs 300 --lr 3e-4

# Change model size
python3 -m moveit_controls.il.train --hidden 512
```

| Flag | Default | Description |
|------|---------|-------------|
| `--episodes_dir` | `~/demonstrations/episodes` | Path to recorded episodes |
| `--output_dir` | `~/models/bc_cube_lift` | Where to save the trained model |
| `--epochs` | 200 | Training epochs |
| `--batch_size` | 64 | Batch size |
| `--lr` | 1e-4 | Learning rate |
| `--hidden` | 256 | Hidden layer size |

Training takes **under 1 minute on CPU**. The model is small (~70K parameters).

Output:
```
Loaded 35 episodes (3 skipped), 2847 timesteps
Train: 2562, Val: 285
Epoch    1/200  train_loss=0.982341  val_loss=0.954123
Epoch   10/200  train_loss=0.412556  val_loss=0.438921
...
Best val_loss: 0.087234
Checkpoint saved to: /home/user/models/bc_cube_lift/best.pt
```

### If Training Loss Doesn't Decrease

- **Too few demos** — Record more (aim for 30+)
- **Inconsistent demos** — Re-record with more consistent technique
- **Try higher learning rate** — `--lr 3e-4` or `--lr 1e-3`

---

## Step 3: Deploy the Policy

### In MuJoCo (simulation only)

```bash
ros2 launch arm_bringup policy_eval.launch.py
```

The policy runs at 10 Hz. It will:
1. Reach toward the cube
2. Close the gripper
3. Lift the cube
4. If lift succeeds (cube held above threshold for 1 second), execute a hardcoded place sequence

### With Real Arm Mirroring

```bash
ros2 launch arm_bringup policy_eval.launch.py dry_run:=false
```

### Launch Options

| Argument | Default | Description |
|----------|---------|-------------|
| `model_path` | `~/models/bc_cube_lift/best.pt` | Path to trained model |
| `headless` | `false` | Run MuJoCo without GUI |
| `dry_run` | `true` | Log servo commands instead of sending to Arduino |
| `serial_port` | `/dev/ttyUSB0` | Arduino serial port |

### Custom Model Path

```bash
ros2 launch arm_bringup policy_eval.launch.py model_path:=~/models/bc_cube_lift/experiment2/best.pt
```

---

## Iterating

The fastest way to improve performance:

1. Run the policy and observe where it fails
2. Record 10–20 more demos that specifically cover the failure cases
3. Retrain (all demos, not just the new ones)
4. Re-evaluate

Training is cheap (seconds), so the bottleneck is always demo quality and quantity.

---

## File Reference

| File | Purpose |
|------|---------|
| `moveit_controls/demo_recorder.py` | Records human teleop episodes |
| `moveit_controls/il/dataset.py` | PyTorch Dataset for loading episodes |
| `moveit_controls/il/policy.py` | MLP policy network definition |
| `moveit_controls/il/train.py` | Training script |
| `moveit_controls/policy_executor.py` | Runs trained policy as ROS2 node |
| `arm_bringup/launch/policy_eval.launch.py` | Launch file for policy evaluation |
| `mujoco_sim_plugins/src/object_state_plugin.cpp` | Publishes cube position from MuJoCo |
| `mujoco_sim_plugins/src/reset_plugin.cpp` | Randomizes cube on sim reset |
| `~/demonstrations/episodes/` | Recorded episode .pt files |
| `~/models/bc_cube_lift/best.pt` | Trained model checkpoint |
