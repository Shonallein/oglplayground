#pragma once

#include <oglplayground/program.h>

#include "application.h"

class GameOfLifeBehavior : public TestApp::Behavior
{
public:

  GameOfLifeBehavior();
  ~GameOfLifeBehavior();
  
  void setup(TestApp::Application* app) override;
  void update(int width, int height) override;
  void teardown(TestApp::Application* app) override;

private:
  class Impl_;
  std::unique_ptr<Impl_> impl_;
};
