#pragma once

#include <stdint.h>

#include "STL/System/System.h"

namespace STL
{
    template<typename T>
    class List
    {
    public:

        void Append(T const& NewElement)
        {        
            uint64_t NewSize = (this->Length() + 1) * sizeof(T);
            T* NewStart = (T*)Malloc(NewSize);

            for (int i = 0; i < this->Length(); i++)
            {
                NewStart[i] = this->Start[i];
            }
            NewStart[this->Length()] = NewElement;

            if (this->Start != nullptr)
            {
                Free(this->Start);
            }

            this->Start = NewStart;
            this->Size = NewSize;
        }

        uint64_t Length() const
        {
            return this->Size / sizeof(T);
        }   

        void Clear()
        {
            Free(this->Start);
            this->Start = nullptr;
            this->Size = 0;
        }

        void Erase(uint64_t Index)
        {
            if (Index > Length() || this->Start == nullptr)
            {
                return;
            }

            uint64_t NewSize = (this->Length() - 1) * sizeof(T);
            T* NewStart = (T*)Malloc(NewSize);

            for (int i = 0; i < Index; i++)
            {
                NewStart[i] = this->Start[i];
            }
            for (int i = Index + 1; i < this->Length(); i++)
            {
                NewStart[i - 1] = this->Start[i];
            }

            this->Start = NewStart;
            this->Size = NewSize;
        }

        T& Last()
        {            
            return Start[Length() - 1];
        }

        T& operator[](uint64_t Index)
        {
            if (Index > Length())
            {
                return Start[Length()];
            }
            return Start[Index];
        }

        List()
        {
            this->Start = nullptr;
            this->Size = 0;
        }

        ~List()
        {
            this->Clear();
        }

    private:

        T* Start = nullptr;

        uint64_t Size = 0;
    };  
}