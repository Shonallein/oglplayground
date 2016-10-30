#include <oglplayground/transform.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OglPlayground
{

const glm::vec3 Transform::worldForward = glm::vec3(0.f, 0.f, 1.f);
const glm::vec3 Transform::worldUp = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 Transform::worldRight = glm::vec3(1.f, 0.f, 0.f);

glm::vec3 Transform::forward() const {
  return glm::rotate(rotation(), worldForward);
}

glm::vec3 Transform::up() const {
  return glm::rotate(rotation(), worldUp);
}

glm::vec3 Transform::right() const {
  return glm::rotate(rotation(), worldRight);
}

const glm::vec3& Transform::localPosition() const {
  return position_;
}

const glm::quat& Transform::localRotation() const {
  return rotation_;
}

const glm::vec3& Transform::localScale() const {
  return scale_;
}

void Transform::setLocalPosition(const glm::vec3& position) {
  position_ = position;
}

void Transform::setLocalRotation(const glm::quat& rotation) {
  rotation_ = rotation;
}

void Transform::setLocalScale(const glm::vec3& scale) {
  scale_ = scale;
}

glm::vec3 Transform::position() const {
  return localPosition();
}

glm::quat Transform::rotation() const {
  return localRotation();
}

glm::vec3 Transform::scale() const {
  return localScale();
}

glm::mat4 Transform::localToWorldMatrix() const {
  glm::mat4 trs = glm::translate(glm::mat4(1.0f), position_);
  trs *= glm::mat4_cast(rotation_);
  trs *= glm::scale(glm::mat4(1.0f), scale_);
  return trs;
}

glm::mat4 Transform::worldToLocalMatrix() const {
  return glm::inverse(localToWorldMatrix());
}

void Transform::translate(const glm::vec3& translation, Space space) {
  if(space == Space::Local) {
    position_ += right()*translation.x+up()*translation.y+forward()*translation.z;
  } else if(space == Space::World) {
    position_ += translation;
  } else {
    assert(false && "Unsupported space");
  }
}

void Transform::rotate(const glm::vec3& eulerAngles, Space space) {
  if(space == Space::Local) {
    rotation_ = rotation_ * glm::quat(glm::radians(eulerAngles));
  } else if(space == Space::World) {
    rotation_ = rotation_ * glm::quat(glm::radians(eulerAngles));
  } else {
    assert(false && "Unsupported space");
  }
}

void Transform::scale(const glm::vec3& scale) {
  scale_ *= scale;
}

} // namespace OglPlayground
