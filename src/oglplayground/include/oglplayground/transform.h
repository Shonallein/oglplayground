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
  static const glm::vec3 forward;
  static const glm::vec3 up;
  static const glm::vec3 right;

  void translate(const glm::vec3& translation, Space space);

  glm::mat4 matrix() const;
  
  glm::vec3 position = glm::vec3(0.f);
  glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
  glm::vec3 scale = glm::vec3(1.f);
};
  
} // namespace OglPlayground
