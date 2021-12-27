#include "cstr.h"
#include "../Math/Math.h"

namespace cstr
{
    char IntToStringOutput[128];
    char* ToString(uint64_t Number)
    {
        uint32_t DigitAmount = Math::GetDigitAmount(Number);

        for (uint32_t i = DigitAmount; i-- > 0;)
        {
            IntToStringOutput[DigitAmount - i - 1] = '0' + Math::GetDigit(Number, i);
        }
        IntToStringOutput[DigitAmount] = '\0';

        return IntToStringOutput;
    }
}
