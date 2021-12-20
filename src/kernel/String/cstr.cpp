#include "cstr.h"
#include "../Math/Math.h"

namespace cstr
{
    char IntToChar(uint8_t Number)
    {
        return (char)Number + '0';
    }

    char* ToString(uint64_t Number)
    {
        return ToString((int64_t)Number);
    }

    char IntToStringOutput[128];
    char* ToString(int64_t Number)
    {
        uint32_t DigitAmount = Math::GetDigitAmount(Number);

        bool IsNegative = false;
        if (Number < 0)
        {
            Number *= -1;
            IsNegative = true;
            IntToStringOutput[0] = '-';
        }

        for (uint32_t i = DigitAmount; i-- > 0;)
        {
            IntToStringOutput[DigitAmount - i - 1 + IsNegative] = IntToChar(Math::GetDigit(Number, i));
        }
        IntToStringOutput[DigitAmount + IsNegative] = '\0';

        return IntToStringOutput;
    }

    char FloatToStringOutput[128];
    char* ToString(float Number, uint32_t DecimalAmount)
    {
        char* IntPtr = (char*)ToString((int64_t)Number);
        char* FloatPtr = FloatToStringOutput;

        if (Number < 0)
        {
            Number *= -1;
        }

        while (*IntPtr != '\0')
        {
            *FloatPtr = *IntPtr;
            IntPtr++;
            FloatPtr++;
        }

        *FloatPtr = '.';
        FloatPtr++;

        Number -= (int)Number;

        for (uint32_t i = 0; i < DecimalAmount; i++)
        {
            Number *= 10.0f;
            *FloatPtr = (int)Number + '0';
            Number -= (int)Number;
            FloatPtr++;
        }

        *FloatPtr = '\0';

        return FloatToStringOutput;
    }
}
