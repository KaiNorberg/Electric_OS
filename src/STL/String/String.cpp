#include "String.h"
#include "cstr.h"

#include "STL/System/System.h"
#include "STL/Memory/Memory.h"

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
        char* NewData = (char*)Malloc(OtherLength + 1);
        CopyString(NewData, Other);
        NewData[OtherLength] = 0;
        
        if (this->Data != nullptr)
        {
            Free(this->Data);
        }

        this->Size = OtherLength;
        this->Data = NewData;
    }

    void String::operator+=(const char* Other)
    {
        uint64_t OtherLength = STL::Length(Other);
        char* NewData = (char*)Malloc(this->Size + OtherLength + 1);
        CopyString(NewData, this->Data);
        CopyString((char*)((uint64_t)NewData + this->Size), Other);
        NewData[this->Size + OtherLength] = 0;
        
        if (this->Data != nullptr)
        {
            Free(this->Data);
        }

        this->Size = this->Size + OtherLength;
        this->Data = NewData;
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