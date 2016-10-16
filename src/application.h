#pragma once

#include <memory>

namespace OglPlayground
{

class InputListener
{
public:
  InputListener() = default;
  ~InputListener() = default;

  virtual void keyEvent(int modifiers, int key, int action) {}
  virtual void mouseMoveEvent(double x, double y) {}
  virtual void scrollEvent(double x, double y) {}
};

class Behavior
{
public:
  Behavior() = default;
  ~Behavior() = default;
  
  virtual void setup() = 0;
  virtual void update(int width, int height) = 0;
  virtual void teardown() = 0;
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
  
} // namespace OglPlayground
