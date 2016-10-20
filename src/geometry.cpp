#include "geometry.h"

#include <algorithm>
#include <cassert>

namespace OglPlayground
{

size_t strideFromVertexDesc(const VertexDesc& desc)
{
  const size_t floatSize = sizeof(float);
  size_t stride = 0;
  for(const auto& attribDesc : desc) {
    stride += attribDesc.nbComponents * floatSize;
  }
  return stride;
}

Geometry::Geometry(
    const float* vertices,
    size_t verticesCount,
    const uint32_t* indices,
    size_t indicesCount,
    const VertexDesc& desc)
    : vertices_(
          GL_ARRAY_BUFFER,
          verticesCount*strideFromVertexDesc(desc),
          vertices,
          GL_STATIC_DRAW)
    , indices_(
        GL_ELEMENT_ARRAY_BUFFER,
        indicesCount*sizeof(uint32_t),
        indices,
        GL_STATIC_DRAW)
    , verticesCount_(verticesCount)
    , indicesCount_(indicesCount)
    , desc_(desc)
{
}

GeometryBinder::GeometryBinder(const Geometry* geometry, AttributeBindDesc desc) : geometry_(geometry)
{
  assert(geometry_ != nullptr);
      
  glGenVertexArrays(1, &vao_);
  bind();
  geometry_->vertices_.bind();
  geometry_->indices_.bind();

  std::sort(
      desc.begin(),
      desc.end(),
      [](const auto& a, const auto& b) { return a.usage < b.usage;});

  const size_t stride = strideFromVertexDesc(geometry->desc_);
  size_t offset = 0;
  for(const auto& attribDesc : geometry->desc_) {
    auto it = std::lower_bound(
        desc.begin(),
        desc.end(),
        attribDesc.usage,
        [](const auto& a, const auto &b) {return a.usage < b;});
    assert(it != desc.end());
    if(it == desc.end()) continue;
    const size_t index = it->index;
    glVertexAttribPointer((GLuint)index, (GLint)attribDesc.nbComponents, GL_FLOAT, GL_FALSE, (GLsizei)stride, (GLvoid*)offset);
    glEnableVertexAttribArray((GLuint)index);
    offset+=attribDesc.nbComponents*sizeof(float);
  }
  
  unbind();
  geometry->vertices_.unbind();
  geometry->indices_.unbind();
}

void GeometryBinder::bind() const
{
  glBindVertexArray(vao_);
}

void GeometryBinder::draw() const
{
  glDrawElements(GL_TRIANGLES, (GLsizei)geometry_->indicesCount_, GL_UNSIGNED_INT, 0);
}

void GeometryBinder::unbind() const
{
  glBindVertexArray(0);
}

} // namespace OglPlayground 
