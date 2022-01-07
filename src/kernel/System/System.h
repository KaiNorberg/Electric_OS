#pragma once

#include <stdint.h>

#include "STL/System/System.h"

#define OS_VERSION "Electric_OS 06/01/2022"

namespace System
{
    STL::SYSRV Call(uint64_t Selector...);
}