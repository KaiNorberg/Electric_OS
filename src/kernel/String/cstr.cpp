#include "cstr.h"
#include "../Math/Math.h"

namespace cstr
{
    char IntToStringOutput[128];
    char* ToString(uint64_t Number)
    {
        uint8_t size;
        uint64_t sizeTest = Number;
        while (sizeTest / 10 > 0)
        {
            sizeTest /= 10;
            size++;
        }

        uint8_t index = 0;
        while(Number / 10 > 0)
        {
            uint8_t remainder = Number % 10;
            Number /= 10;
            IntToStringOutput[size - index] = remainder + '0';
            index++;
        }
        uint8_t remainder = Number % 10;
        IntToStringOutput[size - index] = remainder + '0';
        IntToStringOutput[size + 1] = 0; 
        return IntToStringOutput;
    }
}
