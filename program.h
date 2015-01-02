#pragma once

#include <memory>
#include <vector>
#include "enum.h"

namespace OglPlayground
{

//! @brief Program attribute <-> element binding
struct AttributeBind
{
  ElementUsage usage;
  const char* name;
};
typedef std::vector<AttributeBind> AttributeBinds;

//! Encapsulate an opengl program
class Program
{
public:
  Program(const char* vsSource, const char* psSource, const AttributeBinds& signature);
  ~Program();

  bool isValid() const;
  const std::string& errorString() const;
  
private:
  friend class ProgramBinder;
  
  class Impl_;
  std::unique_ptr<Impl_> impl_;
};

//! Scope bind a Program
class ProgramBinder
{
public:
  ProgramBinder(Program* program);
  ~ProgramBinder();
};

} // namespace OglPlayground
