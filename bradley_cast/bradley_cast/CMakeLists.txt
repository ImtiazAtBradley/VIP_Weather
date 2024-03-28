cmake_minimum_required(VERSION 3.1...3.29)

project(
  BradleyCast
  VERSION 0.1
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(bradley-cast-lib STATIC 
  env_msg.c
)
if(WIN32)
  target_link_libraries(bradley-cast-lib wsock32)
endif()

add_subdirectory("./test/")
