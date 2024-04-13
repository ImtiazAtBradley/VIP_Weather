# TEST EXECUTABLE
add_executable(bradley-cast-test "main.c")

target_link_libraries(bradley-cast-test PRIVATE bradley-cast-lib)

target_include_directories(bradley-cast-test PRIVATE "../")
