cmake_minimum_required (VERSION 3.6)
project (OglPlayground)

# Thirdparty setup
set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(THIRDPARTY_INSTALL_DIR ${THIRDPARTY_DIR}/install)
set(THIRDPARTY_BUILD_DIR ${THIRDPARTY_DIR}/build)
set(CMAKE_PREFIX_PATH
  ${THIRDPARTY_INSTALL_DIR}
  ${THIRDPARTY_INSTALL_DIR}/lib/cmake
  ${CMAKE_PREFIX_PATH}
)

# Build thirdparties
file(MAKE_DIRECTORY ${THIRDPARTY_BUILD_DIR})
message("Building thirdparties")
execute_process(
  COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ${THIRDPARTY_DIR}
  -DTHIRDPARTY_INSTALL_DIR=${THIRDPARTY_INSTALL_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  WORKING_DIRECTORY ${THIRDPARTY_BUILD_DIR}
  RESULT_VARIABLE _res
  OUTPUT_VARIABLE _output
  ERROR_VARIABLE _err
  )
message("${_output}")
message("${_err}")
if(NOT ${_res} EQUAL 0)
  message(FATAL_ERROR "Thirdparty configure stage failed")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${THIRDPARTY_BUILD_DIR}
  RESULT_VARIABLE _res
  OUTPUT_VARIABLE _output
  ERROR_VARIABLE _err
  )
message("${_output}")
message("${_err}")
if(NOT ${_res} EQUAL 0)
  message(FATAL_ERROR "Thirdparty build stage failed")
endif()

# Find thirdparties
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(gtest REQUIRED)

enable_testing()

add_definitions(-DGLM_FORCE_LEFT_HANDED)

add_subdirectory(src/oglplayground)
add_subdirectory(src/testapp)