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
#include <glm/gtc/type_ptr.hpp>

#include "application.h"
#include "geometry.h"
#include "program.h"
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

class TestBehavior : public OglPlayground::Behavior, public OglPlayground::noncopyable
{
public:
  TestBehavior() = default;
  ~TestBehavior() = default;

  void setup() override;
  void update(int width, int height) override;
  void teardown() override;

private:
  std::unique_ptr<OglPlayground::Geometry> geom_;
  std::unique_ptr<OglPlayground::GeometryBinder> geomBinder_;
  std::unique_ptr<OglPlayground::Program> program_;
  GLuint texture_ = 0;
};

void TestBehavior::setup()
{
  std::cout << "SETUP" << std::endl;
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
  geomBinder_.reset(new OglPlayground::GeometryBinder(*geom_, attribDesc));

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
}

void TestBehavior::update(int width, int height)
{
  // Render
  // Clear the colorbuffer
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program_->use();

  glm::mat4 view;
  GLfloat radius = 10.0f;
  GLfloat camX = (GLfloat)sin(glfwGetTime()) * radius;
  GLfloat camZ = (GLfloat)cos(glfwGetTime()) * radius;
  view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection;
  projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

  std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
  };

  glBindTexture(GL_TEXTURE_2D, texture_);
  geomBinder_->bind();
  for(int i = 0; i < cubePositions.size(); ++i)
  {
    glm::mat4 model;
    model = glm::translate(model, cubePositions[i]);
    model = glm::rotate(model, (GLfloat)glfwGetTime()*glm::radians(20.0f * (i+1)), glm::vec3(1.0f, 0.3f, 0.5f));
    glm::mat4 mvp = projection*view*model;
    glUniformMatrix4fv(program_->desc("transform")->location, 1, GL_FALSE, glm::value_ptr(mvp));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
  geomBinder_->unbind();
}

void TestBehavior::teardown()
{
  std::cout << "TEARDOWN" << std::endl;
  geomBinder_.reset(nullptr);
  geom_.reset(nullptr);
  program_.reset(nullptr);
}

class InputLogger : public OglPlayground::InputListener, public OglPlayground::noncopyable
{
public:
  InputLogger() = default;
  ~InputLogger() = default;

  void keyEvent(int modifiers, int key, int action) override {
    std::cout << "KEY EVENT (modifiers:" << modifiers << ", key:" << key << ", action:" << action << ")" << std::endl;
  }
  void mouseMoveEvent(double x, double y) override {
    std::cout << "MOUSE MOVE EVENT (x:" << x << ", y:" << y << ")" << std::endl;
  }
  void scrollEvent(double x, double y) override {
    std::cout << "SCROLL EVENT (x:" << x << ", y:" << y << ")" << std::endl;
  }
};

}

int main()
{
  OglPlayground::Application application;
  InputLogger inputLogger;
  application.registerListener(&inputLogger);
  TestBehavior behavior;
  application.run(&behavior);
  application.unregisterListener(&inputLogger);
}
