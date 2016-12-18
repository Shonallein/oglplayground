#include "gameoflifebehavior.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <stb/stb_image.h>

#include <oglplayground/geometry.h>

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

class FullscreenQuad
{
public:
  FullscreenQuad() {
    const GLfloat vertices[] = {
      -1.f, 3.f, 0.f, 2.0f,
      -1.f, -1.f, 0.f, 0.0f,
      3.f, -1.f, 2.f, 0.0f,
    };
    const uint32_t indices[] = { 0, 1, 2 };

    const OglPlayground::VertexDesc vertDesc = {
      {OglPlayground::AttributeUsage::Position, 2},
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
  }

  void draw() {
    geomBinder_->bind();
    geomBinder_->draw();
    geomBinder_->unbind();
  }
private:
  std::unique_ptr<OglPlayground::Geometry> geom_;
  std::unique_ptr<OglPlayground::GeometryBinder> geomBinder_;
};

}

class GameOfLifeBehavior::Impl_
{
public:
  // Display data
  std::unique_ptr<OglPlayground::Program> program;
  std::unique_ptr<FullscreenQuad> fullscreenQuad;
  GLuint texture = 0;
  std::unique_ptr<OglPlayground::BufferObject> pbo;

  // Game of life data
  size_t width = 1024;
  size_t height = 1024;
};

GameOfLifeBehavior::GameOfLifeBehavior() : impl_(new Impl_) {}
GameOfLifeBehavior::~GameOfLifeBehavior() {}

void GameOfLifeBehavior::setup(TestApp::Application* app)
{

  // Init quad and shader to display the gol state
  impl_->fullscreenQuad.reset(new FullscreenQuad);
  impl_->program = loadShader_("fullscreen");
  assert(impl_->program->isValid());

  // Generate the result texture
  glGenTextures(1, &impl_->texture);
  glBindTexture(GL_TEXTURE_2D, impl_->texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)impl_->width, (GLsizei)impl_->height);
  impl_->pbo.reset(new OglPlayground::BufferObject(GL_PIXEL_UNPACK_BUFFER, impl_->width*impl_->height*4, nullptr, GL_STREAM_DRAW));
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GameOfLifeBehavior::update(int width, int height)
{
  glViewport(0, 0, width, height);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  impl_->program->use();
  glBindTexture(GL_TEXTURE_2D, impl_->texture);
  impl_->pbo->bind();
  uint8_t* ptr = static_cast<uint8_t*>(impl_->pbo->map(GL_WRITE_ONLY));
  for(size_t i = 0; i < impl_->width; ++i) {
    for(size_t j = 0; j < impl_->width; ++j) {
      *ptr++ = rand() % 255;
      *ptr++ = rand() % 255;
      *ptr++ = rand() % 255;
      *ptr++ = 255;
    }
  }
  impl_->pbo->unmap();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)impl_->width, (GLsizei)impl_->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  impl_->pbo->unbind();

  impl_->fullscreenQuad->draw();
}

void GameOfLifeBehavior::teardown(TestApp::Application* app)
{
  impl_->program.reset(nullptr);
}
