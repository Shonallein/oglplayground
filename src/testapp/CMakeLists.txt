set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(src/resources_path.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/resources_path.cpp @ONLY)

set(SOURCES
  src/application.cpp
  src/main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resources_path.cpp
  src/sphericalcontroller.cpp

  # Behaviors
  src/gameoflifebehavior.cpp
  src/testbehavior.cpp
  )

add_executable(testapp ${SOURCES})
target_include_directories(testapp PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(testapp PUBLIC glfw oglplayground docopt_s)