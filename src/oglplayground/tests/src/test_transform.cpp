#include <gtest/gtest.h>
#include <oglplayground/transform.h>

using OglPlayground::Transform;

namespace glm
{

::std::ostream& operator<<(::std::ostream& os, const vec3& vec) {
  return os
      << "(x:" << vec.x << ", " 
      << "y:" << vec.y << ", " 
      << "z:" << vec.z << ")"; 
}

::std::ostream& operator<<(::std::ostream& os, const quat& rot) {
  return os
      << "(x:" << rot.x << ", " 
      << "y:" << rot.y << ", " 
      << "z:" << rot.z << ", " 
      << "w:" << rot.w << ")"; 
}

} // namespace glm

TEST(TransformTest, Default) {
  Transform tf;
  EXPECT_EQ(glm::vec3(0.f), tf.position);
  EXPECT_EQ(glm::quat(1.f, 0.f, 0.f, 0.f), tf.rotation);
  EXPECT_EQ(glm::vec3(1.f), tf.scale);
}
