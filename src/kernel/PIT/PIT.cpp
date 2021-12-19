#include "PIT.h"

#include "../IO/IO.h"

namespace PIT
{
    volatile uint64_t Ticks = 0.0;

    uint16_t Divisor = 65535;

    void Sleep(double Seconds)
    {	
        double EndTicks = Ticks + GetFrequency() * Seconds;
        while (Ticks < EndTicks)
        {
            asm("HLT");
        }
    }

    uint64_t GetFrequency()
    {
        return PIT_FREQUENCY / Divisor;
    }

    void SetFrequency(uint64_t Frequency)
    {
        SetDivisor(PIT_FREQUENCY / Frequency);
    }

    void SetDivisor(uint16_t NewDivisor)
    {
        Divisor = NewDivisor;
        IO::OutByte(0x40, (uint8_t)Divisor);
        IO::IOWait();
        IO::OutByte(0x40, (uint8_t)(Divisor >> 8));
    }

    void Tick()
    {
        Ticks++;
    }
}