#pragma once

#include <memory>

struct GLFWwindow;

namespace TestApp
{

class InputListener
{
public:
  InputListener() = default;
  ~InputListener() = default;

  virtual void keyEvent(GLFWwindow* window, int modifiers, int key, int action) {}
  virtual void mouseButtonEvent(GLFWwindow* window, int modifiers, int button, int action) {}
  virtual void mouseMoveEvent(GLFWwindow* window, double x, double y) {}
  virtual void scrollEvent(GLFWwindow* window, double x, double y) {}
};

class Application;

class Behavior
{
public:
  Behavior() = default;
  ~Behavior() = default;
  
  virtual void setup(Application* app) = 0;
  virtual void update(int width, int height) = 0;
  virtual void teardown(Application* app) = 0;
};
  
class Application
{
public:
  Application();
  ~Application();

  void run(Behavior* behavior);

  void registerListener(InputListener* listener);
  void unregisterListener(InputListener* listener);

private:
  class Impl_;
  std::unique_ptr<Impl_> impl_;
};
  
} // namespace TestApp
