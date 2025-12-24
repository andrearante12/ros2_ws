file(REMOVE_RECURSE
  "include/local_planner_parameters.hpp"
  "include/moveit_hybrid_planning/local_planner_parameters.hpp"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/local_planner_parameters.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
