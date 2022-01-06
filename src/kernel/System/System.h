#pragma once

#include <stdint.h>

#include "../../slib/System/System.h"

#define OS_VERSION "Electric_OS 01/01/2022"

namespace System
{
    const char* System(const char* Input);

    slib::SYSRV Call(uint64_t Selector...);
}