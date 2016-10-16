#pragma once

#include <inttypes.h>
#include <vector>

#include <glad/glad.h>

#include "bufferobject.h"
#include "noncopyable.h"

namespace OglPlayground
{

enum class AttributeUsage
{
  Position,
  UV0
};

struct VertexAttribute
{
  AttributeUsage usage;
  size_t nbComponents; // nb floats used for this Attribute
};
typedef std::vector<VertexAttribute> VertexDesc;
size_t strideFromVertexDesc(const VertexDesc& desc);

class Geometry : public noncopyable
{
public:
  friend class GeometryBinder;
  
  Geometry(
      const float* vertices,
      size_t verticesCount,
      const uint32_t* indices,
      size_t indicesCount,
      const VertexDesc& desc);

private:
  BufferObject vertices_; 
  BufferObject indices_;
  VertexDesc desc_;
};

struct AttributeBind
{
  AttributeUsage usage;
  size_t index;
};
typedef std::vector<AttributeBind> AttributeBindDesc;

class GeometryBinder : public noncopyable
{
public:
  GeometryBinder(const Geometry& geometry, AttributeBindDesc desc);
  void bind() const;
  void unbind() const;

private:
  GLuint vao_ = 0;
};

} // namespace OglPlayground
