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

ExternalProject_Add(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG dca0a37ff492200451b4a489276fcd9a065afc2d
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND cd ../stb && cmake -E make_directory ${THIRDPARTY_INSTALL_DIR}/include/stb && cmake -E copy_if_different stb_image.h ${THIRDPARTY_INSTALL_DIR}/include/stb 
  )

ExternalProject_Add(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.8.1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${THIRDPARTY_INSTALL_DIR}
  )

ExternalProject_Add(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${THIRDPARTY_INSTALL_DIR} -Dgtest_force_shared_crt=ON
  )
