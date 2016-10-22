#pragma once

#include <glm/glm.hpp>

#include "transform.h"

namespace OglPlayground
{

class Camera
{
public:
  Camera() = default;
  ~Camera() = default;
  Camera(const Camera& other) = default;
  Camera& operator=(const Camera& other) = default;

  void setFov(float fov);
  float fov() const;

  void setAspect(float aspect);
  float aspect() const;

  void setClippingPlanes(float near, float far);
  float nearPlane() const;
  float farPlane() const;

  const glm::mat4& projection() const;
  
  Transform& transform();

private:

  void flagDirty();
  
  Transform transform_;
  float fov_ = 45.f;
  float aspect_ = 4.f/3.f;
  float nearPlane_ = 0.f;
  float farPlane_ = 100.f;

  mutable bool cacheDirty_ = true;
  mutable glm::mat4 projection_;
};
  
} // namespace OglPlayground
