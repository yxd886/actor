FILE(REMOVE_RECURSE
  "CMakeFiles/all_tools"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/all_tools.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
