#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OglPlayground
{

class Transform
{
public:  
  glm::vec3 position = glm::vec3(0.f);
  glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
  glm::vec3 scale = glm::vec3(1.f);

  glm::mat4 matrix() const;
};
  
} // namespace OglPlayground
