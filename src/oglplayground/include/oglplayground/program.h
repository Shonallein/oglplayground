#pragma once

#include <functional>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/fwd.hpp>

#include "noncopyable.h"

namespace OglPlayground
{

struct UniformDesc
{
  GLint type = 0;
  GLint location = 0;
  std::string name;
};

//! Encapsulate an opengl program
class Program : public noncopyable
{
public:
  typedef std::function<void (const char*)> LogFunction;
  Program(const char* vsSource, const char* psSource, const LogFunction& log = {});
  Program(Program&& other);
  ~Program();

  Program& operator=(Program&& other);

  void swap(Program& other);

  bool isValid() const;
  void use() const;

  const UniformDesc* desc(const char* name) const;
  // List returned in alphabetic order
  const std::vector<UniformDesc>& descs() const;

  void setUniform(const char* name, float v);
  void setUniform(const char* name, const glm::mat4& v);
  
private:
  GLuint program_ = 0;
  std::vector<UniformDesc> uniformsDesc_;
};

} // namespace OglPlayground
