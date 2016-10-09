#pragma once

#include <functional>
#include <vector>
#include <string>

#include <glad/glad.h>

namespace OglPlayground
{

struct UniformDesc
{
  GLint type;
  GLint location;
  std::string name;
};

//! Encapsulate an opengl program
class Program
{
public:
  typedef std::function<void (const char*)> LogFunction;
  Program(const char* vsSource, const char* psSource, const LogFunction& log = {});
  ~Program();

  bool isValid() const;
  void use() const;

  // List returned in alphabetic order
  const std::vector<UniformDesc>& descs() const;
  
private:
  GLuint program_ = 0;
  std::vector<UniformDesc> uniformsDesc_;
};

} // namespace OglPlayground
