#pragma once

#include <stdint.h>

#include "DeviceHeader.h"

#include "kernel/ACPI/ACPI.h"

namespace PCI
{            
    /// <summary>
    //// Starts over enumeration to the first device.
    /// </summary>
    void ResetEnumeration();

    /// <summary>
    //// Sets out to be the next PCI device and returns true untill there are no more devices then it returns fales.
    /// </summary>
    bool Enumerate(SDTHeader* MCFG, DeviceHeader*& Out);
}