#include <oglplayground/transform.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OglPlayground
{

const glm::vec3 Transform::forward = glm::vec3(0.f, 0.f, -1.f);
const glm::vec3 Transform::up = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 Transform::right = glm::vec3(1.f, 0.f, 0.f);

void Transform::translate(const glm::vec3& translation, Space space)
{
  //TODO
}

glm::mat4 Transform::matrix() const
{
  glm::mat4 trs = glm::scale(glm::mat4(1.0f), scale);
  trs *= glm::mat4_cast(rotation);
  trs *= glm::translate(glm::mat4(1.0f), position);
  return trs;
}

}
