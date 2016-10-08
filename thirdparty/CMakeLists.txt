cmake_minimum_required (VERSION 3.6)

include(ExternalProject)
ExternalProject_Add(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.2.1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${THIRDPARTY_INSTALL_DIR}
  )
