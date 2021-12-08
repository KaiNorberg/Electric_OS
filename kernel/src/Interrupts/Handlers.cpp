#include "Handlers.h"
#include "IDT.h"
#include "../Rendering/Renderer.h"
#include "../Core/Panic.h"
#include "../IO/IO.h"

namespace InteruptHandlers
{
    __attribute__((interrupt)) void InvalidOP(struct InterruptFrame* frame)
    {
        Panic("Invalid OP Code Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DeviceNotDetected(struct InterruptFrame* frame)
    {
        Panic("Device Not Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DoubleFault(struct InterruptFrame* frame)
    {
        Panic("Double Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void SegmentNotPresent(struct InterruptFrame* frame)
    {
        Panic("Segment Not Present");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void StackSegmentFault(struct InterruptFrame* frame)
    {
        Panic("Stack Segment Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void GeneralProtectionFault(struct InterruptFrame* frame)
    {
        Panic("General Protection Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void PageFault(struct InterruptFrame* frame)
    {
        Panic("Page Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void Keyboard(struct InterruptFrame* frame)
    {
        Renderer::Print("Press\n\r");
        uint8_t ScanCode = IO::InByte(0x60);

        IO::OutByte(PIC1_COMMAND, PIC_EOI);
    }
}
