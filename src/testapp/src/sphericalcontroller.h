#pragma once

#include <memory>

#include <glm/fwd.hpp>

#include <oglplayground/noncopyable.h>

#include "application.h"

namespace OglPlayground
{
  class Transform;
} // namespace OglPlayground

namespace TestApp
{

class InputListener;
  
class SphericalController : public TestApp::InputListener, public OglPlayground::noncopyable
{
public:
  SphericalController(OglPlayground::Transform* transform, const glm::vec3& center);
  ~SphericalController();

  void mouseButtonEvent(GLFWwindow* window, int modifiers, int button, int action) override;
  void mouseMoveEvent(GLFWwindow*, double x, double y) override;
  void scrollEvent(GLFWwindow*, double x, double y) override;

private:
  class Impl_;
  std::unique_ptr<Impl_> impl_;
};

} // namespace TestApp
