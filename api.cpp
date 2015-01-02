#include "api.h"
#include "gl_core_3_3.h"

namespace OglPlayground
{

bool initApi()
{
  return ogl_LoadFunctions() != ogl_LOAD_FAILED;
}
  
} // namespace OglPlayground
