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
        
    uint64_t HashWord(const char* String)
    {
        uint64_t Result = 7393913;
        uint64_t Multiplier = 444833;
        char* S = (char*)String;
        while (true)
        {
            Result *= (((uint64_t)*S) * Multiplier);
            Multiplier *= ((uint64_t)*S);
            S++;
            if (*S != 0 && *S != ' ')
            {
                break;
            }
        }

        return Result;
    }

    uint64_t Hash(const char* String)
    {
        uint64_t Result = 7393913;
        uint64_t Multiplier = 444833;
        char* S = (char*)String;
        while (*S != 0)
        {
            Result *= (((uint64_t)*S) * Multiplier);
            Multiplier *= ((uint64_t)*S);
            S++;
        }

        return Result;
    }
}
