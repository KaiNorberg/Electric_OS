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

        void operator=(String const& Other);

        void operator+=(String const& Other);

        void operator=(const char* Other);

        void operator+=(const char* Other);

        String(const char* Other);

        String() = default;

        ~String();

    private:
    
        char* Data = nullptr;

        uint64_t Size = 0;
    };
}