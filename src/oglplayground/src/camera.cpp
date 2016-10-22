#include <oglplayground/camera.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OglPlayground
{

void Camera::setFov(float fov)
{
  fov_ = fov;
  flagDirty();
}

float Camera::fov() const
{
  return fov_;
}

void Camera::setAspect(float aspect)
{
  aspect_ = aspect;
  flagDirty();
}

float Camera::aspect() const
{
  return aspect_;
}

void Camera::setClippingPlanes(float nearPlane, float farPlane)
{
  nearPlane_ = nearPlane;
  farPlane_ = farPlane;
  flagDirty();
}

float Camera::nearPlane() const
{
  return nearPlane_;
}

float Camera::farPlane() const
{
  return farPlane_;
}

const glm::mat4& Camera::projection() const
{
  if (cacheDirty_) {
    projection_ = glm::perspective(fov_, aspect_, nearPlane_, farPlane_);
    cacheDirty_ = false;
  }
  return projection_;
}

Transform& Camera::transform()
{
  return transform_;
}

void Camera::flagDirty()
{
  cacheDirty_ = true;
}

} // namespace OglPlayground
