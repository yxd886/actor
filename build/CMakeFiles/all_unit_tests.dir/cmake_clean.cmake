FILE(REMOVE_RECURSE
  "CMakeFiles/all_unit_tests"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/all_unit_tests.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
