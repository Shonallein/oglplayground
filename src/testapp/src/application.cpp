#include "application.h"

#include <cassert>
#include <iostream>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace TestApp
{

class Application::Impl_
{
public:
  Impl_() { assert(currentInst_ == nullptr); currentInst_ = this;}
  ~Impl_() {currentInst_ = nullptr;}
  
  GLFWwindow* window_;
  std::set<InputListener*> listeners_;

  // Input management
  static Impl_* currentInst_;
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    assert(currentInst_ != nullptr);
    if(currentInst_ == nullptr) return;
    
    (void)scancode;
    for(auto listener : currentInst_->listeners_) {
      listener->keyEvent(window, modifiers, key, action);
    }
  }

  static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers) {
    assert(currentInst_ != nullptr);
    if(currentInst_ == nullptr) return;

    for(auto listener : currentInst_->listeners_) {
      listener->mouseButtonEvent(window, modifiers, button, action);
    }
  }
  
  static void cursor_pos_callback(GLFWwindow* window, double x, double y) {
    assert(currentInst_ != nullptr);
    if(currentInst_ == nullptr) return;

    for(auto listener : currentInst_->listeners_) {
      listener->mouseMoveEvent(window, x, y);
    }
  }
  
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    assert(currentInst_ != nullptr);
    if(currentInst_ == nullptr) return;

    for(auto listener : currentInst_->listeners_) {
      listener->scrollEvent(window, xoffset, yoffset);
    }
  }
};
Application::Impl_* Application::Impl_::currentInst_ = nullptr;

Application::Application() : impl_(new Impl_)
{
  // Init GLFW
  glfwInit();
  
  // Setup GLFW flags
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  impl_->window_ = glfwCreateWindow(800, 800, "OglPlayground", NULL, NULL);
  if (impl_->window_ == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(impl_->window_);
  
  // Setup OpenGL Loader
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return;
  }

  // Input callbacks setup
  glfwSetKeyCallback(impl_->window_, &Impl_::key_callback);
  glfwSetMouseButtonCallback(impl_->window_, &Impl_::mouse_button_callback);
  glfwSetCursorPosCallback(impl_->window_, &Impl_::cursor_pos_callback);
  glfwSetScrollCallback(impl_->window_, &Impl_::scroll_callback);
}

Application::~Application()
{
  glfwTerminate();
}

void Application::run(Behavior* behavior)
{
  assert(behavior);
  behavior->setup(this);
  while (!glfwWindowShouldClose(impl_->window_))
  {
    // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    int width, height;
    glfwGetWindowSize(impl_->window_, &width, &height);
    behavior->update(width, height);
    glfwSwapBuffers(impl_->window_);
  }
  behavior->teardown(this);
}

void Application::registerListener(InputListener* listener)
{
  impl_->listeners_.insert(listener);
}

void Application::unregisterListener(InputListener* listener)
{
  impl_->listeners_.erase(listener);
}

} // namespace TestApp
