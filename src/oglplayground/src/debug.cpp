#include <oglplayground/debug.h>

#include <glm/gtx/string_cast.hpp>

#define GLM_TYPE_STREAM_OP_IMPL(type) \
  namespace glm \
  { \
    ::std::ostream& operator<<(::std::ostream& os, const glm::##type& val) {    \
    return os << glm::to_string(val); \
    } \
  } // namespace glm

GLM_TYPE_STREAM_OP_IMPL(vec3)
GLM_TYPE_STREAM_OP_IMPL(quat)
GLM_TYPE_STREAM_OP_IMPL(mat3)
GLM_TYPE_STREAM_OP_IMPL(mat4)
