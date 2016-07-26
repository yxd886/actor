FILE(REMOVE_RECURSE
  "CMakeFiles/hello_world1.dir/hello_world1.cpp.o"
  "../bin/hello_world1.pdb"
  "../bin/hello_world1"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/hello_world1.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
