#pragma once

#include <stdint.h>

#include "STL/System/System.h"

namespace System
{
    STL::SYSRV Call(uint8_t Selector...);
}