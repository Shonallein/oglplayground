#pragma once

#include <memory>

#include <glad/glad.h>

#include <oglplayground/camera.h>
#include <oglplayground/geometry.h>
#include <oglplayground/program.h>

#include "application.h"
#include "sphericalcontroller.h"

class TestBehavior : public TestApp::Behavior, public OglPlayground::noncopyable
{
public:
  TestBehavior() = default;
  ~TestBehavior() = default;

  void setup(TestApp::Application* app) override;
  void update(int width, int height) override;
  void teardown(TestApp::Application* app) override;

private:
  std::unique_ptr<OglPlayground::Geometry> geom_;
  std::unique_ptr<OglPlayground::GeometryBinder> geomBinder_;
  std::unique_ptr<OglPlayground::Program> program_;
  OglPlayground::Camera camera_;
  std::unique_ptr<TestApp::SphericalController> sphericalController_;
  GLuint texture_ = 0;
};
