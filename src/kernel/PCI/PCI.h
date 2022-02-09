#pragma once

#include <stdint.h>

#include "kernel/ACPI/ACPI.h"
#include "DeviceHeader.h"

namespace PCI
{        
    void ResetEnumeration();

    bool Enumerate(MCFGHeader* MCFG, DeviceHeader*& Out);
}