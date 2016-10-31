#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_TYPE_STREAM_OP_DEC(type) \
  namespace glm \
  { \
    ::std::ostream& operator<<(::std::ostream& os, const glm::##type& val); \
  } // namespace glm

GLM_TYPE_STREAM_OP_DEC(vec3)
GLM_TYPE_STREAM_OP_DEC(quat)
GLM_TYPE_STREAM_OP_DEC(mat3)
GLM_TYPE_STREAM_OP_DEC(mat4)
