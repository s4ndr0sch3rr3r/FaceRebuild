# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FaceRebuild_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FaceRebuild_autogen.dir\\ParseCache.txt"
  "FaceRebuild_autogen"
  )
endif()
