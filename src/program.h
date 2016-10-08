#pragma once

#include <functional>

#include <glad/glad.h>

namespace OglPlayground
{

//! Encapsulate an opengl program
class Program
{
public:
  typedef std::function<void (const char*)> LogFunction;
  Program(const char* vsSource, const char* psSource, const LogFunction& log = {});
  ~Program();

  bool isValid() const;
  void use() const;
  
private:
  GLuint program_ = 0;
};

} // namespace OglPlayground
