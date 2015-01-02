#pragma once
#include "gl_core_3_3.h"

namespace OglPlayground
{

//! Init the opengl library.
//! Will load and ensure that the opengl library is available
//! @return false if the initialization failed otherwise true
bool initApi();
  
} // namespace OglPlayground
