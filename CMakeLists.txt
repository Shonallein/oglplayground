cmake_minimum_required (VERSION 2.8.12)
project (OglPlayground)
set(CMAKE_CXX_FLAGS "-std=c++11")

# External project setup
set(THIRDPARTY_DIR ${CMAKE_BINARY_DIR}/thirdparty)
set(CMAKE_PREFIX_PATH
  ${THIRDPARTY_DIR}
  ${CMAKE_PREFIX_PATH}
)

# External project configure macro
macro(custom_external_project name)
  set(_external_dir ${CMAKE_BINARY_DIR}/external_project/${name})
  configure_file(
	${name}.external_project.txt
	${_external_dir}/CMakeLists.txt
	[@ONLY]
	)
  file(MAKE_DIRECTORY ${_external_dir}/build)
  execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ..
    WORKING_DIRECTORY ${_external_dir}/build
	RESULT_VARIABLE _res
	OUTPUT_VARIABLE _output
	ERROR_VARIABLE _err
    )
  message("${name} configure stage res: ${_res}")
  message("${_output}")
  message("${_err}")
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${_external_dir}/build
	RESULT_VARIABLE _res
	OUTPUT_VARIABLE _output
	ERROR_VARIABLE _err
    )
  message("${name} build stage res: ${_res}")
  message("${_output}")
  message("${_err}")
endmacro()

# Add here your external projects
custom_external_project(glfw)

# Find required modules
find_package(glfw REQUIRED)
find_package(OpenGL REQUIRED)

message(${GLFW_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

set(SOURCES
  gl_core_3_3.c
  main.cpp
)
add_executable(OglPlayground ${SOURCES})
target_link_libraries(OglPlayground ${OPENGL_LIBRARIES} ${GLFW_LIBRARY} Xxf86vm pthread X11 Xrandr Xi)