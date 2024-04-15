cmake_minimum_required(VERSION 3.10)

project(bradley-cast-broker)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (SRCS 
	"./main.cpp"
	"./src/broker.cpp"
	"./src/broker_database.cpp"
	"./src/MessageParse.cpp"
)

set (INCLUDE_DIRS 
	"./include"
)

# Add executable with sources
add_executable(bradley-cast-broker ${SRCS})

target_include_directories(bradley-cast-broker PRIVATE ${INCLUDE_DIRS})

# Windows only - add location for linux headers later
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# C Redis Client
	target_link_libraries(bradley-cast-broker PRIVATE hiredis)
else ()
	message(FATAL_ERROR "Build only supported for linux")
endif()

# C math
# target_link_libraries(target m)

# Set include directory
# target_include_directories(target PUBLIC include_dir)
