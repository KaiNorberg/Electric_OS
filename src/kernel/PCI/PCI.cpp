#include "PCI.h"

#include "kernel/Memory/Paging/PageTable.h"

namespace PCI
{       
    void EnumerateFunction(uint64_t DeviceAddress, uint64_t Function)
    {    
        uint64_t FunctionAddress = DeviceAddress + (Function << 12);

        PageTableManager::MapAddress((void*)FunctionAddress, (void*)FunctionAddress);

        DeviceHeader* Header = (DeviceHeader*)FunctionAddress;

        if (Header->DeviceID == 0 || Header->DeviceID == 0xFFFF)
        {
            return;
        }
    }

    void EnumerateDevice(uint64_t BusAddress, uint64_t Device)
    {
        uint64_t DeviceAddress = BusAddress + (Device << 15);

        PageTableManager::MapAddress((void*)DeviceAddress, (void*)DeviceAddress);

        DeviceHeader* Header = (DeviceHeader*)DeviceAddress;

        if (Header->DeviceID == 0 || Header->DeviceID == 0xFFFF)
        {
            return;
        }

        for (uint64_t Func = 0; Func < 8; Func++)
        {
            EnumerateFunction(BusAddress, Func);
        }
    }

    void EnumerateBus(uint64_t BaseAddress, uint64_t Bus)
    {
        uint64_t BusAddress = BaseAddress + (Bus << 20);

        PageTableManager::MapAddress((void*)BusAddress, (void*)BusAddress);

        DeviceHeader* Header = (DeviceHeader*)BusAddress;

        if (Header->DeviceID == 0 || Header->DeviceID == 0xFFFF)
        {
            return;
        }    

        for (uint64_t Device = 0; Device < 32; Device++)
        {
            EnumerateDevice(BusAddress, Device);
        }
    }

    void Enumerate(MCFGHeader* MCFG)
    {
        uint64_t Entries = (MCFG->Header.Length - sizeof(MCFGHeader)) / sizeof(DeviceConfig);

        for (uint64_t i = 0; i < Entries; i++)
        {
            DeviceConfig* NewDeviceConfig = (DeviceConfig*)((uint64_t)MCFG + sizeof(MCFGHeader) + i * sizeof(DeviceConfig));

            for (uint64_t Bus = NewDeviceConfig->StartBus; Bus < NewDeviceConfig->EndBus; Bus++)
            {
                EnumerateBus(NewDeviceConfig->Base, Bus);
            }
        }
    }
}