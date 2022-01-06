#pragma once

#include <stdint.h>

#include "kernel/../STL/System/System.h"

#define OS_VERSION "Electric_OS 06/01/2022"

namespace System
{
    const char* System(const char* Input);

    STL::SYSRV Call(uint64_t Selector...);
}