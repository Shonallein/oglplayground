#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <oglplayground/camera.h>
#include <oglplayground/debug.h>
#include <oglplayground/geometry.h>
#include <oglplayground/program.h>
#include <oglplayground/transform.h>

#include "application.h"
#include "resources_path.h"

namespace
{

std::unique_ptr<OglPlayground::Program> loadShader_(const std::string& filename)
{
  std::string vertSrc;
  {
    std::ifstream t(OglPlayground::resource_path(filename+".vert.glsl"));
    std::stringstream buffer;
    buffer << t.rdbuf();
    vertSrc = buffer.str();
  }

  std::string fragSrc;
  {
    std::ifstream t(OglPlayground::resource_path(filename+".frag.glsl"));
    std::stringstream buffer;
    buffer << t.rdbuf();
    fragSrc = buffer.str();
  }

  return std::unique_ptr<OglPlayground::Program>(
      new OglPlayground::Program(vertSrc.c_str(), fragSrc.c_str(), [](const char* msg) { std::cerr << msg; }));
}


class SphericalTransformController : public OglPlayground::InputListener, public OglPlayground::noncopyable
{
public:
  SphericalTransformController(OglPlayground::Transform* transform, const glm::vec3& center)
      : transform_(transform)
  {
    assert(transform_);
    transform_->translate(glm::vec3(0.f, 0.f, -5.f), OglPlayground::Space::Local);
    transform_->lookAt(center, OglPlayground::Transform::worldUp);
    distance_ = glm::length(center-transform_->position());
  }

  //void keyEvent(int modifiers, int key, int action) override {}
  void mouseMoveEvent(double x, double y) override {
    if(init_) {      
      const glm::vec3 translation(0.f, 0.f, distance_);
      transform_->translate(translation, OglPlayground::Space::Local);
      transform_->rotate(glm::vec3((y-lastMouseY_)*speed_, (x-lastMouseX_)*speed_, 0.f), OglPlayground::Space::Local);
      transform_->translate(-translation, OglPlayground::Space::Local);
      
      transform_->lookAt(sphericalTransform_.position(), OglPlayground::Transform::worldUp);
    }
    lastMouseX_ = x;
    lastMouseY_ = y;
    init_ = true;
  }
  
  void scrollEvent(double x, double y) override {
    transform_->translate(glm::vec3(0.f, 0.f, y), OglPlayground::Space::Local);
    std::cout << transform_->localPosition() << std::endl;
  }

private:
  OglPlayground::Transform* transform_;
  OglPlayground::Transform sphericalTransform_;

  float distance_;
  glm::vec3 center_;
  float speed_ = 1.f;

  
  float lastMouseX_ = 0.f;
  float lastMouseY_ = 0.f;
  bool init_ = false;
};

class TestBehavior : public OglPlayground::Behavior, public OglPlayground::noncopyable
{
public:
  TestBehavior();
  ~TestBehavior() = default;

  void setup(OglPlayground::Application* app) override;
  void update(int width, int height) override;
  void teardown(OglPlayground::Application* app) override;

private:
  std::unique_ptr<OglPlayground::Geometry> geom_;
  std::unique_ptr<OglPlayground::GeometryBinder> geomBinder_;
  std::unique_ptr<OglPlayground::Program> program_;
  OglPlayground::Camera camera_;
  SphericalTransformController sphericalController_;
  GLuint texture_ = 0;
};

TestBehavior::TestBehavior() : sphericalController_(&camera_.transform(), glm::vec3(0.f)) {}

void TestBehavior::setup(OglPlayground::Application* app)
{
  std::cout << "SETUP" << std::endl;

  app->registerListener(&sphericalController_);
  
  // geomtery 
  const GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
    
  const uint32_t indices[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
  };

  const OglPlayground::VertexDesc vertDesc = {
    {OglPlayground::AttributeUsage::Position, 3},
    {OglPlayground::AttributeUsage::UV0, 2}
  };
  const size_t stride = strideFromVertexDesc(vertDesc);
  geom_.reset(
      new OglPlayground::Geometry(
          vertices,
          sizeof(vertices)/stride,
          indices,
          sizeof(indices)/sizeof(uint32_t),
          vertDesc));

  const OglPlayground::AttributeBindDesc attribDesc = {
    {OglPlayground::AttributeUsage::Position, 0},
    {OglPlayground::AttributeUsage::UV0, 1}
  };
  geomBinder_.reset(new OglPlayground::GeometryBinder(geom_.get(), attribDesc));

  // Shader
  program_ = loadShader_("simple");
  assert(program_->isValid());
  for(const auto& desc : program_->descs())
  {
    std::cout << "Uniform " << desc.name << " located at " << desc.location << std::endl;
  }

  // Load texture (TODO CLEANUP)
  int width, height, nbChannel;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* image = stbi_load(OglPlayground::resource_path("container.jpg"), &width, &height, &nbChannel, STBI_rgb);
  assert(nbChannel == 3);
  
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(image);

  // Camera setup
  camera_.setFov(45.f);
  camera_.setClippingPlanes(0.1f, 100.f);
}

void TestBehavior::update(int width, int height)
{
  // Render
  // Clear the colorbuffer
  camera_.setAspect((float)width / (float)height);
  glViewport(0, 0, width, height);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program_->use();
  glBindTexture(GL_TEXTURE_2D, texture_);
  geomBinder_->bind();

  glm::mat4 view(1.f);
  GLfloat radius = 5.0f;
  GLfloat camX = (GLfloat)sin(glfwGetTime()) * radius;
  GLfloat camZ = (GLfloat)cos(glfwGetTime()) * radius;
  //view = glm::lookAt(glm::vec3(radius, radius, radius), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  view = camera_.transform().worldToLocalMatrix();
  
  glm::mat4 model(1.f);
  glm::mat4 mvp = camera_.projection()*view*model;
  glUniformMatrix4fv(program_->desc("transform")->location, 1, GL_FALSE, glm::value_ptr(mvp));
  geomBinder_->draw();
  geomBinder_->unbind();
}

void TestBehavior::teardown(OglPlayground::Application* app)
{
  std::cout << "TEARDOWN" << std::endl;
  app->unregisterListener(&sphericalController_);
  geomBinder_.reset(nullptr);
  geom_.reset(nullptr);
  program_.reset(nullptr);
}

}

int main()
{
  OglPlayground::Application application;
  TestBehavior behavior;
  application.run(&behavior);
}
