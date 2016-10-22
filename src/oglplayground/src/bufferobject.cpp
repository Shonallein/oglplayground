#include <oglplayground/bufferobject.h>

namespace OglPlayground
{

BufferObject::BufferObject(GLenum target, size_t size, const void* data, GLenum usage)
    : target_(target)
    , size_(size)
{
  glGenBuffers(1, &buffer_);
  bind();
  glBufferData(target, size, data, usage);
  unbind();
}

BufferObject::~BufferObject()
{
  glDeleteBuffers(1, &buffer_);
}

GLuint BufferObject::name() const
{
  return buffer_;
}

void BufferObject::bind() const
{
  glBindBuffer(target_, buffer_);
}

void BufferObject::unbind() const
{
  glBindBuffer(target_, 0);
}

} // namespace OglPlayground
