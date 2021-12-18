#include "Handlers.h"
#include "IDT.h"
#include "SystemCalls.h"
#include "../Rendering/Renderer.h"
#include "../Core/Panic.h"
#include "../IO/IO.h"
#include "../UserInput/KeyBoard.h"
#include "../UserInput/Mouse.h"

namespace InteruptHandlers
{
    __attribute__((interrupt)) void InvalidOP(InterruptFrame* frame)
    {
        KernelPanic("Invalid OP Code Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DeviceNotDetected(InterruptFrame* frame)
    {
        KernelPanic("Device Not Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DoubleFault(InterruptFrame* frame)
    {
        KernelPanic("Double Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void SegmentNotPresent(InterruptFrame* frame)
    {
        KernelPanic("Segment Not Present");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void StackSegmentFault(InterruptFrame* frame)
    {
        KernelPanic("Stack Segment Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void GeneralProtectionFault(InterruptFrame* frame)
    {
        KernelPanic("General Protection Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void PageFault(InterruptFrame* frame)
    {
        KernelPanic("Page Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void Keyboard(InterruptFrame* frame)
    {
        uint8_t ScanCode = IO::InByte(0x60);

        KeyBoard::HandleScanCode(ScanCode);

        IO::OutByte(PIC1_COMMAND, PIC_EOI);
    }

    __attribute__((interrupt)) void Mouse(InterruptFrame* frame)
    {
        uint8_t MouseData = IO::InByte(0x60);

        Mouse::HandleMouseData(MouseData);

        IO::OutByte(PIC2_COMMAND, PIC_EOI);
        IO::OutByte(PIC1_COMMAND, PIC_EOI);
    }

    __attribute__((interrupt)) void SystemCall(InterruptFrame* frame)
    {
        SystemCalls::SysCall();
    }
}
