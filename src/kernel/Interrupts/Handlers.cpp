#include "Handlers.h"
#include "IDT.h"
#include "../Rendering/Renderer.h"
#include "../Debug/Debug.h"
#include "../IO/IO.h"
#include "../RTC/RTC.h"
#include "../UserInput/KeyBoard.h"
#include "../UserInput/Mouse.h"
#include "../PIT/PIT.h"

namespace InteruptHandlers
{    
    __attribute__((interrupt)) void BoundRange(InterruptFrame* frame)
    {
        Debug::Error("Bound Range Exceeded");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void InvalidOP(InterruptFrame* frame)
    {
        Debug::Error("Invalid OP Code Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DeviceNotDetected(InterruptFrame* frame)
    {
        Debug::Error("Device Not Detected");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void DoubleFault(InterruptFrame* frame)
    {
        Debug::Error("Double Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void InvalidTSS(InterruptFrame* frame)
    {
        Debug::Error("Invalid TSS");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void SegmentNotPresent(InterruptFrame* frame)
    {
        Debug::Error("Segment Not Present");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void StackSegmentFault(InterruptFrame* frame)
    {
        Debug::Error("Stack Segment Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void GeneralProtectionFault(InterruptFrame* frame)
    {
        Debug::Error("General Protection Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void PageFault(InterruptFrame* frame)
    {
        Debug::Error("Page Fault");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void FloatingPoint(InterruptFrame* frame)
    {
        Debug::Error("Floating Point Exception");
        while(true)
        {
            asm("HLT");
        }
    }

    __attribute__((interrupt)) void PIT(InterruptFrame* frame)
    {
        PIT::Tick();
        if (PIT::GetFrequency() % 100 == 0)
        {
            RTC::Update();
        }

        IO::OutByte(PIC1_COMMAND, PIC_EOI);
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
}
