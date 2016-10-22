#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gtest/gtest.h>
#include <oglplayground/transform.h>

using OglPlayground::Transform;

#define GLM_TYPE_STREAM_OP(type) \
  namespace glm \
  { \
    ::std::ostream& operator<<(::std::ostream& os, const glm::##type& val) {    \
    return os << glm::to_string(val); \
    } \
  } // namespace glm

GLM_TYPE_STREAM_OP(vec3)
GLM_TYPE_STREAM_OP(quat)
GLM_TYPE_STREAM_OP(mat3)
GLM_TYPE_STREAM_OP(mat4)

TEST(TransformTest, RightHanded) {
  EXPECT_EQ(glm::vec3(1.f, 0.f, 0.f), Transform::right);
  EXPECT_EQ(glm::vec3(0.f, 1.f, 0.f), Transform::up);
  EXPECT_EQ(glm::vec3(0.f, 0.f, -1.f), Transform::forward);
}

TEST(TransformTest, GlmConversions) {
  glm::mat3 defaultRot(Transform::right, Transform::up, Transform::forward);
  std::cout << defaultRot << std::endl;
  glm::quat quat = glm::normalize(glm::toQuat(defaultRot));
  std::cout << quat << std::endl; 
  std::cout << glm::mat4_cast(quat) << std::endl; 
}

TEST(TransformTest, Default) {
  Transform tf;
  EXPECT_EQ(glm::vec3(0.f), tf.position);
  glm::mat3 defaultRot(Transform::right, Transform::up, Transform::forward);
  EXPECT_EQ(glm::toQuat(defaultRot), tf.rotation);
  EXPECT_EQ(glm::vec3(1.f), tf.scale);
}
