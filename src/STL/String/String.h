#pragma once

#include <stdint.h>

namespace STL
{
    class String
    {
    public:

        char Pop();

        char* cstr() const;

        uint64_t Length() const;

        void Erase(uint64_t Index, uint64_t Amount = 1);

        void Reserve(uint64_t MinSize);

        void operator+=(char const& Other);

        void operator=(String const& Other);

        void operator+=(String const& Other);

        void operator=(const char* Other);

        void operator+=(const char* Other);

        char& operator[](uint64_t Index); 

        const char& operator[](uint64_t Index) const; 

        String(const char* Other);

        String() = default;

        ~String();

    private:
    
        char* Data = nullptr;

        uint64_t Size = 0;

        uint64_t ReservedSize = 0;
    };
}