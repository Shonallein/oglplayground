#include "sphericalcontroller.h"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <oglplayground/transform.h>

namespace TestApp
{

class SphericalController::Impl_
{
public:
  Impl_() = default;
  ~Impl_() = default;

  OglPlayground::Transform* transform_ = nullptr;

  glm::vec3 center_;
  float distance_ = 0.f;
  glm::vec3 angles_;
  float speed_ = .1f;

  
  double lastMouseX_ = 0.f;
  double lastMouseY_ = 0.f;
  bool pressed_ = false;
};

SphericalController::SphericalController(OglPlayground::Transform* transform, const glm::vec3& center)
    : impl_(new Impl_)
{
  impl_->transform_ = transform;
  impl_->center_ = center;
  
  assert(transform);
  impl_->transform_->lookAt(center, OglPlayground::Transform::worldUp);
  impl_->distance_ = glm::length(center-impl_->transform_->position());
  impl_->angles_ = glm::eulerAngles(impl_->transform_->rotation());
}

SphericalController::~SphericalController() {}

void SphericalController::mouseButtonEvent(
    GLFWwindow* window, int modifiers, int button, int action)
{
  if(button != GLFW_MOUSE_BUTTON_LEFT) return;
  impl_->pressed_ = (action == GLFW_PRESS);
  glfwGetCursorPos(window, &impl_->lastMouseX_, &impl_->lastMouseY_);
}
  
void SphericalController::mouseMoveEvent(GLFWwindow*, double x, double y) {
  if(!impl_->pressed_) return;
    
  const glm::vec3 translation(0.f, 0.f, impl_->distance_);
  impl_->transform_->translate(translation, OglPlayground::Space::Local);
  impl_->angles_ +=
      glm::vec3(
          float(y-impl_->lastMouseY_) * impl_->speed_ * impl_->distance_,
          float(x-impl_->lastMouseX_) * impl_->speed_ * impl_->distance_,
          0.f);
  impl_->angles_.x = std::max(std::min(impl_->angles_.x, 89.f), -89.f);
  impl_->transform_->setLocalRotation(glm::quat(glm::radians(impl_->angles_)));
  impl_->transform_->translate(-translation, OglPlayground::Space::Local);
  impl_->transform_->lookAt(impl_->center_, OglPlayground::Transform::worldUp);
    
  impl_->lastMouseX_ = x;
  impl_->lastMouseY_ = y;
}
  
void SphericalController::scrollEvent(GLFWwindow*, double x, double y) {
  glm::vec3 translation(0.f, 0.f, float(y));
  impl_->distance_ += float(y);
  impl_->transform_->translate(-translation, OglPlayground::Space::Local);
}

} //namespace TestApp
