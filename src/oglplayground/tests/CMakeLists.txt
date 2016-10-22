# First small lib for glad
add_executable(oglplayground_test src/main.cpp src/test_transform.cpp)
target_link_libraries(oglplayground_test PUBLIC oglplayground GTest::GTest GTest::Main)

add_test(oglplayground_test oglplayground_test)

