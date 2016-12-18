#include "application.h"

#include <cassert>
#include <iostream>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace TestApp
{

namespace
{
void myDebugOutput(GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
}

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
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

  GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
     glEnable(GL_DEBUG_OUTPUT);
     glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
     glDebugMessageCallback(myDebugOutput, nullptr);
     glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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
