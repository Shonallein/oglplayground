#include <string>
#include <iostream>

#include "api.h"
#include "debug.h"
#include "program.h"

namespace OglPlayground
{

namespace
{

GLuint createShaderFromSrc(GLenum type, const char* src, std::string& log)
{
  // Create Ogl shader object
  GLuint shader = glCreateShader(type);
  OGL_ASSERT();
  glShaderSource(shader, 1, &src, NULL);
  OGL_ASSERT();
  // Compile the shader
  glCompileShader(shader);
  OGL_ASSERT();
  // Check if the compilation succeeded
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  OGL_ASSERT();
  if(status == GL_TRUE) return shader;

  // Something went wrong, get the compilation log
  GLint logLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  OGL_ASSERT();
  std::vector<char> strLog(logLength+1);
  glGetShaderInfoLog(shader, logLength, NULL, &strLog[0]);
  OGL_ASSERT();
  log = std::string(&strLog[0]);
  
  // Delete allocated shader an return null
  glDeleteShader(shader);
  OGL_ASSERT();
  return 0u;
}

} // anonymous namespace

////////////////////////////////////////////////////////////////////////
////////////////////////////// Program /////////////////////////////////
////////////////////////////////////////////////////////////////////////
class Program::Impl_
{
public:
  Impl_() : program_(glCreateProgram()) {
    OGL_ASSERT();
  }
  ~Impl_() {
    glDeleteProgram(program_);
    OGL_ASSERT();
  }
  std::string errorString_;
  GLuint program_;
};

Program::Program(
    const char* vsSource,
    const char* psSource,
    const AttributeBinds& signature)
    : impl_(new Impl_())
{
  // Try to compile the vertex shader
  GLuint vs = 0;
  {
    std::string log;
    vs = createShaderFromSrc(GL_VERTEX_SHADER, vsSource, log);
    impl_->errorString_ += log;
    if(vs == 0) return; // Compilation failed
    // Associate the shaders to the program
    glAttachShader(impl_->program_, vs);
    OGL_ASSERT();
  }

  // Try to compile the fragment shader
  GLuint ps = 0;
  {
    std::string log;
    ps = createShaderFromSrc(GL_FRAGMENT_SHADER, psSource, log);
    impl_->errorString_ += log;
    if(ps == 0) return; // Compilation failed
    // Associate the shaders to the program
    glAttachShader(impl_->program_, ps);
    OGL_ASSERT();
  }

  // Link the program
  glLinkProgram(impl_->program_);
  OGL_ASSERT();

  // Release the shaders
  glDeleteShader(vs);
  OGL_ASSERT();
  glDeleteShader(ps);
  OGL_ASSERT();
  
  // Check if the link succeed
  GLint status; 
  glGetProgramiv(impl_->program_, GL_LINK_STATUS, &status);
  OGL_ASSERT();

  // Grab the log if the link failed
  if(status != GL_TRUE) {
    GLint logLength;
    glGetShaderiv(impl_->program_, GL_INFO_LOG_LENGTH, &logLength);
    OGL_ASSERT();
    std::vector<char> strLog(logLength+1);
    glGetShaderInfoLog(impl_->program_, logLength, NULL, &strLog[0]);
    OGL_ASSERT();
    impl_->errorString_ += std::string(&strLog[0]);
    return;
  }

  // TODO Fetch uniform description
}

Program::~Program()
{
}

bool Program::isValid() const
{
  return impl_->errorString_.empty();
}

const std::string& Program::errorString() const
{
  return impl_->errorString_;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////// ProgramBinder ///////////////////////////
////////////////////////////////////////////////////////////////////////
ProgramBinder::ProgramBinder(Program* program)
{
  glUseProgram(program->impl_->program_);
  OGL_ASSERT();
}

ProgramBinder::~ProgramBinder()
{
  glUseProgram(0);
  OGL_ASSERT();
}

} //namespace oglPlayground
