#include "program.h"

#include <cassert>
#include <vector>

namespace OglPlayground
{
namespace
{

struct Shader
{
  Shader() {};
  Shader(GLenum type) : id(glCreateShader(type)) {}
  Shader(Shader&& other) : id(other.id) { other.id = 0; }
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  ~Shader() {
	  if (id != 0) {
		  glDeleteShader(id);
	  }
  }
  GLuint id = 0;
};

Shader createShaderFromSrc_(GLenum type, const char* src, const Program::LogFunction& log)
{
  // Create Ogl shader object and specify the shader sources
  Shader shader(type);
  glShaderSource(shader.id, 1, &src, NULL);
  // Compile the shader
  glCompileShader(shader.id);

  // log compilation info
  if(log) {
    GLint logLength;
    glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> strLog(logLength+1);
    glGetShaderInfoLog(shader.id, logLength, NULL, &strLog[0]);
    log(&strLog[0]);
  }
  
  // Check if the compilation succeeded
  GLint status;
  glGetShaderiv(shader.id, GL_COMPILE_STATUS, &status);
  if(status == GL_TRUE) return shader;
  
  return Shader();
}

} // anonymous namespace

Program::Program(
    const char* vsSource,
    const char* psSource,
    const LogFunction& log)
{

  std::vector<Shader> shaders;
  // Try to compile the vertex shader
  shaders.push_back(createShaderFromSrc_(GL_VERTEX_SHADER, vsSource, log));
  if(shaders.back().id == 0) return; // Compilation failed

  // Try to compile the fragment shader
  shaders.push_back(createShaderFromSrc_(GL_FRAGMENT_SHADER, psSource, log));
  if(shaders.back().id == 0) return; // Compilation failed

  // Create the program and attach the compiled shader to it
  program_ = glCreateProgram();
  for(const auto & shader : shaders) {
    glAttachShader(program_, shader.id);
  }
  
  // Link the program
  glLinkProgram(program_);

  // log link info
  if(log) {
    GLint logLength;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> strLog(logLength+1);
    glGetProgramInfoLog(program_, logLength, NULL, &strLog[0]);
    log(&strLog[0]);
  }
  
  // Check if the link succeed
  GLint status; 
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if(status == GL_TRUE) return;

  glDeleteProgram(program_);
      
  // TODO Fetch uniform description
}

Program::~Program()
{
  if(program_ != 0) glDeleteProgram(program_);
}

bool Program::isValid() const
{
  return program_ != 0;
}

void Program::use() const
{
  assert(isValid());
  glUseProgram(program_);
}

} //namespace oglPlayground
