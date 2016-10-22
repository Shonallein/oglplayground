#pragma once

#include <vector>

#include <glad/glad.h>

#include "noncopyable.h"

namespace OglPlayground
{

class BufferObject : public noncopyable
{
public:
  BufferObject(GLenum target, size_t size, const void* data, GLenum usage);
  ~BufferObject();

  GLuint name() const;
  void bind() const;
  void unbind() const;

private:
  GLenum target_ = 0;
  size_t size_ = 0;
  GLuint buffer_ = 0;
};
  
} // namespace OglPlayground
