#include "testbehavior.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <stb/stb_image.h>

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

}

void TestBehavior::setup(TestApp::Application* app)
{
  camera_.transform().translate(glm::vec3(0.f, 0.f, -5.f), OglPlayground::Space::Local);
  sphericalController_.reset(new TestApp::SphericalController(&camera_.transform(), glm::vec3(0.f)));

  app->registerListener(sphericalController_.get());
  
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
  
  glm::mat4 view = camera_.transform().worldToLocalMatrix();
  glm::mat4 model(1.f);
  glm::mat4 mvp = camera_.projection()*view*model;
  program_->setUniform("transform", mvp);
  glBindTexture(GL_TEXTURE_2D, texture_);
  
  geomBinder_->bind();
  geomBinder_->draw();
  geomBinder_->unbind();
}

void TestBehavior::teardown(TestApp::Application* app)
{
  app->unregisterListener(sphericalController_.get());
  geomBinder_.reset(nullptr);
  geom_.reset(nullptr);
  program_.reset(nullptr);
}
