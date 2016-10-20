#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OglPlayground
{

glm::mat4 Transform::matrix() const
{
  glm::mat4 trs = glm::scale(glm::mat4(1.0f), scale);
  trs *= glm::mat4_cast(rotation);
  trs *= glm::translate(glm::mat4(1.0f), position);
  return trs;
}

}
