set(SOURCES
  src/application.cpp
  src/main.cpp
  src/sphericalcontroller.cpp
  )

set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(src/resources_path.h.in ${CMAKE_CURRENT_BINARY_DIR}/resources_path.h @ONLY)

add_executable(testapp ${SOURCES})
target_include_directories(testapp PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(testapp PUBLIC glfw oglplayground)