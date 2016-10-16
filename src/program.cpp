#include "program.h"

#include <algorithm>
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
  if(status == GL_FALSE) {
    glDeleteProgram(program_);
    program_ = 0;
    return;
  };

  // Fetch uniform description
  GLint numUniforms = 0;
  glGetProgramInterfaceiv(program_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
  const GLenum properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};
  uniformsDesc_.reserve(numUniforms);
  for(int unif = 0; unif < numUniforms; ++unif)
  {
    GLint values[4];
    glGetProgramResourceiv(program_, GL_UNIFORM, unif, 4, properties, 4, NULL, values);
    
    // TODO: handle uniforms that are in a block.
    if(values[0] != -1) continue;

    uniformsDesc_.emplace_back();
    auto& desc = uniformsDesc_.back();
    desc.type = values[1];
    desc.location = values[3];
    desc.name.resize(values[2]);
    glGetProgramResourceName(
        program_,
        GL_UNIFORM,
        unif,
        (GLsizei)desc.name.size(),
        NULL,
        const_cast<char*>(desc.name.data()));
  }

  std::sort(
      uniformsDesc_.begin(),
      uniformsDesc_.end(),
      [](const auto& a, const auto& b) { return a.name < b.name; });
}

Program::Program(Program&& other)
{
  this->swap(other);
}

Program::~Program()
{
  if(program_ != 0) glDeleteProgram(program_);
}

Program& Program::operator=(Program&& other)
{
  this->swap(other);
  return *this;
}

void Program::swap(Program& other)
{
  std::swap(program_, other.program_);
  std::swap(uniformsDesc_, other.uniformsDesc_);
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

const UniformDesc* Program::desc(const std::string& name) const
{
  auto it = std::lower_bound(
      uniformsDesc_.cbegin(),
      uniformsDesc_.cend(),
      name,
      [](const auto& elem, const auto& value) { return elem.name < value; });
  if(it != uniformsDesc_.end()) return &(*it);
  return nullptr;
}
    
const std::vector<UniformDesc>& Program::descs() const
{
  return uniformsDesc_;
}

} //namespace oglPlayground
