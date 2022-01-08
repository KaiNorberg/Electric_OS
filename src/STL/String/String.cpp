#include "String.h"
#include "cstr.h"

#include "STL/System/System.h"
#include "STL/Memory/Memory.h"
#include "STL/Math/Math.h"

namespace STL
{        
    char String::Pop()
    {
        char Temp = this->Data[this->Size - 1];
        this->Data[this->Size - 1] = 0;
        return Temp;
    }

    char* String::cstr() const
    {
        return this->Data;
    }

    uint64_t String::Length() const
    {
        return this->Size;
    }

    void String::Reserve(uint64_t MinSize)
    {
        if (MinSize <= this->ReservedSize)
        {
            return;
        }

        this->ReservedSize = MinSize * 2;    
        char* NewData = (char*)Malloc(ReservedSize);

        if (this->Data != nullptr)
        {        
            for (int i = 0; i < this->Size; i++)
            {
                NewData[i] = this->Data[i];
            }

            Free(this->Data);
        }

        this->Data = NewData;
    }

    void String::operator=(String const& Other)
    {
        this->operator=(Other.cstr());
    }

    void String::operator+=(String const& Other)
    {
        this->operator+=(Other.cstr());
    }

    void String::operator=(const char* Other)
    {       
        uint64_t OtherLength = STL::Length(Other);
        this->Reserve(OtherLength + 1);

        for (int i = 0; i < OtherLength; i++)
        {
            this->Data[i] = Other[i];
        }
        this->Data[OtherLength] = 0;
        this->Size = OtherLength;
    }

    void String::operator+=(const char* Other)
    {
        uint64_t OtherLength = STL::Length(Other);
        this->Reserve(this->Size + OtherLength + 1);

        for (int i = 0; i < OtherLength; i++)
        {
            this->Data[this->Size + i] = Other[i];
        }
        this->Data[this->Size + OtherLength] = 0;
        this->Size += OtherLength;
    }

    char& String::operator[](uint64_t Index)
    {
        return this->Data[Index];
    }

    const char& String::operator[](uint64_t Index) const
    {
        return this->Data[Index];
    }

    String::String(const char* Other)
    {
        this->operator=(Other);
    }

    String::~String()
    {
        if (this->Data != nullptr)
        {
            Free(this->Data);
        }
    }
}