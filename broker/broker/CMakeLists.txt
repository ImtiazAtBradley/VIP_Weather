cmake_minimum_required(VERSION 3.10)

project(bradley-cast-broker)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)

set (SRCS 
	"./main.cpp"
	"./src/broker.cpp"
	"./src/MessageParse.cpp"
	"./src/utils.cpp"
)

set (
	TST_SRCS
	"./src/broker.cpp"
	"./src/MessageParse.cpp"
	"./src/utils.cpp"
	"./test/test.cpp"
)

# Add executable with sources
add_executable(bradley-cast-broker ${SRCS})

# Add test executable
add_executable(bradley-cast-broker-test ${TST_SRCS})

target_include_directories(bradley-cast-broker PUBLIC "include/")
target_include_directories(bradley-cast-broker-test PUBLIC "include/")

# curl library
target_link_libraries(bradley-cast-broker curl)
target_link_libraries(bradley-cast-broker-test curl)

# C math
# target_link_libraries(target m)

# Set include directory
# target_include_directories(target PUBLIC include_dir)
