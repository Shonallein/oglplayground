#pragma once

#include <cassert>

#include "api.h"

#define OGL_ASSERT() assert(glGetError() == GL_NO_ERROR)
