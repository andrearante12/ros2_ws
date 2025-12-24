file(REMOVE_RECURSE
  "include/hp_manager_parameters.hpp"
  "include/moveit_hybrid_planning/hp_manager_parameters.hpp"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/hp_manager_parameters.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
