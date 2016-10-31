#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OglPlayground
{

enum class Space
{
  Local,
  World
};

class Transform
{
public:
  static const glm::vec3 worldForward;
  static const glm::vec3 worldUp;
  static const glm::vec3 worldRight;

  Transform() = default;

  glm::vec3 forward() const;
  glm::vec3 up() const;
  glm::vec3 right() const;

  const glm::vec3& localPosition() const;
  const glm::quat& localRotation() const;
  const glm::vec3& localScale() const;
  void setLocalPosition(const glm::vec3& position);
  void setLocalRotation(const glm::quat& rotation);
  void setLocalScale(const glm::vec3& scale);

  glm::vec3 position() const;
  glm::quat rotation() const;
  glm::vec3 scale() const;

  glm::mat4 localToWorldMatrix() const;
  glm::mat4 worldToLocalMatrix() const;

  void translate(const glm::vec3& translation, Space space);
  void rotate(const glm::vec3& eulerAngles, Space space);
  void lookAt(const glm::vec3& target, glm::vec3 up);
  void scale(const glm::vec3& scale);
  
private:
  glm::vec3 position_ = glm::vec3(0.f);
  glm::quat rotation_ = glm::quat(1.0, 0.0, 0.0, 0.0);
  glm::vec3 scale_ = glm::vec3(1.f);
};
  
} // namespace OglPlayground
