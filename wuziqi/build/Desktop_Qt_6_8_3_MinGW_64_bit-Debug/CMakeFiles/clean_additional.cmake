# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\wuziqi_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\wuziqi_autogen.dir\\ParseCache.txt"
  "wuziqi_autogen"
  )
endif()
