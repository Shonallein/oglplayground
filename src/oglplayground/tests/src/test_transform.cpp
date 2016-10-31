#include <glm/gtx/quaternion.hpp>
#include <gtest/gtest.h>
#include <oglplayground/transform.h>
#include <oglplayground/debug.h>

using OglPlayground::Transform;
using OglPlayground::Space;

TEST(TransformTest, LeftHanded) {
  EXPECT_EQ(glm::vec3(1.f, 0.f, 0.f), Transform::worldRight);
  EXPECT_EQ(glm::vec3(0.f, 1.f, 0.f), Transform::worldUp);
  EXPECT_EQ(glm::vec3(0.f, 0.f, 1.f), Transform::worldForward);
}

struct ExpectedValues
{
  glm::vec3 position;
  glm::quat rotation;
  glm::vec3 scale;
  
  glm::vec3 right;
  glm::vec3 up;
  glm::vec3 forward;

  glm::mat4 localToWorld;
  glm::mat4 worldToLocal;
};

const float fprecision = 1000.f;
bool floatEqualVec3(glm::vec3 a, glm::vec3 b) {
  a = glm::round(a*fprecision) / fprecision;
  b = glm::round(b*fprecision) / fprecision;
  return a == b;
}

bool floatEqualQuat(glm::quat a, glm::quat b) {
  auto rf = [](float f) { return round(f*fprecision) / fprecision; };
  a = glm::quat(rf(a.x),rf(a.y),rf(a.z),rf(a.w));
  b = glm::quat(rf(b.x),rf(b.y),rf(b.z),rf(b.w));
  return a == b;
}

bool floatEqualMat4(glm::mat4 a, glm::mat4 b) {
  auto rf = [](glm::vec4 v) { return glm::round(v*fprecision) / fprecision; };
  a = glm::mat4(rf(a[0]), rf(a[1]), rf(a[2]), rf(a[3]));
  b = glm::mat4(rf(b[0]), rf(b[1]), rf(b[2]), rf(b[3]));
  return a == b;
}

void check(const Transform& tf, const ExpectedValues& expected) {
  EXPECT_PRED2(floatEqualVec3, expected.position, tf.position());
  EXPECT_PRED2(floatEqualQuat, expected.rotation, tf.rotation());
  EXPECT_PRED2(floatEqualVec3, expected.scale, tf.scale());

  EXPECT_PRED2(floatEqualVec3, expected.right, tf.right());
  EXPECT_PRED2(floatEqualVec3, expected.up, tf.up());
  EXPECT_PRED2(floatEqualVec3, expected.forward, tf.forward());

  EXPECT_PRED2(floatEqualMat4, expected.localToWorld, tf.localToWorldMatrix());
  EXPECT_PRED2(floatEqualMat4, expected.worldToLocal, tf.worldToLocalMatrix());
}

TEST(TransformTest, Default) {
  const Transform tf;
  const ExpectedValues expected = {
    glm::vec3(0.0f, 0.0f, 0.0f), // position
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // rotation
    glm::vec3(1.0f, 1.0f, 1.0f), // scale
    glm::vec3(1.0f, 0.0f, 0.0f), // ritgh
    glm::vec3(0.0f, 1.0f, 0.0f), // up
    glm::vec3(0.0f, 0.0f, 1.0f), // forward
    glm::mat4( // local to world
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(0.0f, 0.0f, 0.0f , 1.0f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(0.0f, 0.0f, 0.0f , 1.0f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, TranslateLocalSpace) {
  Transform tf;
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::Local);
  const ExpectedValues expected = {
    glm::vec3(5.0f, 3.0f, 1.0f), // position
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // rotation
    glm::vec3(1.0f, 1.0f, 1.0f), // scale
    glm::vec3(1.0f, 0.0f, 0.0f), // ritgh
    glm::vec3(0.0f, 1.0f, 0.0f), // up
    glm::vec3(0.0f, 0.0f, 1.0f), // forward
    glm::mat4( // local to world
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(5.0f, 3.0f, 1.0f , 1.0f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(-5.0f, -3.0f, -1.0f , 1.0f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, TranslateWorldSpace) {
  Transform tf;
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::World);
  const ExpectedValues expected = {
    glm::vec3(5.0f, 3.0f, 1.0f), // position
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // rotation
    glm::vec3(1.0f, 1.0f, 1.0f), // scale
    glm::vec3(1.0f, 0.0f, 0.0f), // ritgh
    glm::vec3(0.0f, 1.0f, 0.0f), // up
    glm::vec3(0.0f, 0.0f, 1.0f), // forward
    glm::mat4( // local to world
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(5.0f, 3.0f, 1.0f , 1.0f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(1.0f, 0.0f, 0.0f , 0.0f), // 0
        glm::vec4(0.0f, 1.0f, 0.0f , 0.0f), // 1
        glm::vec4(0.0f, 0.0f, 1.0f , 0.0f), // 2
        glm::vec4(-5.0f, -3.0f, -1.0f , 1.0f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, RotateLocalSpace) {
  Transform tf;
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::Local);
  const ExpectedValues expected = {
    glm::vec3(0.000f, 0.000f, 0.000f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(0.000f, 0.000f, 0.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(0.000f, 0.000f, 0.000f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, RotateWorldSpace) {
  Transform tf;
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::World);
  const ExpectedValues expected = {
    glm::vec3(0.000f, 0.000f, 0.000f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(0.000f, 0.000f, 0.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(0.000f, 0.000f, 0.000f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, TLocalRLocal) {
  Transform tf;
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::Local);
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::Local);
  const ExpectedValues expected = {
    glm::vec3(5.000f, 3.000f, 1.000f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(5.000f, 3.000f, 1.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(-2.828f, -3.000f, -4.243f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, TLocalRWorld) {
  Transform tf;
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::Local);
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::World);
  const ExpectedValues expected = {
    glm::vec3(5.000f, 3.000f, 1.000f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(5.000f, 3.000f, 1.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(-2.828f, -3.000f, -4.243f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, RLocalTLocal) {
  Transform tf;
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::Local);
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::Local);
  const ExpectedValues expected = {
    glm::vec3(4.243f, 3.000f, -2.828f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(4.243f, 3.000f, -2.828f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(-5.000f, -3.000f, -1.000f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, RLocalTWorld) {
  Transform tf;
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::Local);
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::World);
  const ExpectedValues expected = {
    glm::vec3(5.000f, 3.000f, 1.000f), // position
    glm::quat(0.924f, 0.000f, 0.383f, 0.000f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(0.707f, 0.000f, -0.707f), // ritgh
    glm::vec3(0.000f, 1.000f, 0.000f), // up
    glm::vec3(0.707f, 0.000f, 0.707f), // forward
    glm::mat4( // local to world
        glm::vec4(0.707f, 0.000f, -0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(5.000f, 3.000f, 1.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(0.707f, 0.000f, 0.707f , 0.000f), // 0
        glm::vec4(0.000f, 1.000f, 0.000f , 0.000f), // 1
        glm::vec4(-0.707f, 0.000f, 0.707f , 0.000f), // 2
        glm::vec4(-2.828f, -3.000f, -4.243f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}

TEST(TransformTest, LookAt) {
  Transform tf;
  tf.translate(glm::vec3(5.f, 3.f, 1.f), Space::World);
  tf.rotate(glm::vec3(0.f, 45.f, 0.f), Space::Local);
  tf.lookAt(glm::vec3(10.f, 0.f, 0.f), Transform::worldUp);
  
  const ExpectedValues expected = {
    glm::vec3(5.000f, 3.000f, 1.000f), // position
    glm::quat(0.612f, 0.167f, 0.746f, -0.203f), // rotation
    glm::vec3(1.000f, 1.000f, 1.000f), // scale
    glm::vec3(-0.196f, 0.000f, -0.981f), // ritgh
    glm::vec3(0.497f, 0.862f, -0.099f), // up
    glm::vec3(0.845f, -0.507f, -0.169f), // forward
    glm::mat4( // local to world
        glm::vec4(-0.196f, 0.000f, -0.981f , 0.000f), // 0
        glm::vec4(0.497f, 0.862f, -0.099f , 0.000f), // 1
        glm::vec4(0.845f, -0.507f, -0.169f , 0.000f), // 2
        glm::vec4(5.000f, 3.000f, 1.000f , 1.000f) // 3
               ),
    glm::mat4( // world to local
        glm::vec4(-0.196f, 0.497f, 0.845f , 0.000f), // 0
        glm::vec4(0.000f, 0.862f, -0.507f , 0.000f), // 1
        glm::vec4(-0.981f, -0.099f, -0.169f , 0.000f), // 2
        glm::vec4(1.961f, -4.972f, -2.535f , 1.000f) // 3
               )
  };
  
  check(tf, expected);
}



