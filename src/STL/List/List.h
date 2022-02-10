#pragma once

#include <stdint.h>

#include "STL/System/System.h"

namespace STL
{
    template<typename T>
    class List
    {
    public:

        void Reserve(uint32_t MinSize)
        {
            if (MinSize <= this->ReservedSize)
            {
                return;
            }

            this->ReservedSize = MinSize * 2;    
            T* NewData = (T*)Malloc(this->ReservedSize);

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

        void Push(T const& NewElement)
        {      
            this->Size++;
            this->Reserve(this->Size);
            this->Data[Size - 1] = NewElement;
        }

        T Pop()
        {      
            this->Size--;
            return this->Data[Size];
        }

        uint32_t Length() const
        {
            return this->Size;
        }   

        void Clear()
        {
            Free(this->Data);
            this->Data = nullptr;
            this->Size = 0;
            this->ReservedSize = 0;
        }

        void Erase(uint32_t Index)
        {
            for (int i = Index + 1; i < this->Size; i++)
            {
                this->Data[i - 1] = this->Data[i];
            }

            this->Size--;
        }

        T& Last()
        {            
            return Data[this->Size - 1];
        }

        T& operator[](uint32_t Index)
        {
            return Data[Index];
        }

        ~List()
        {
            this->Clear();
        }

    private:

        T* Data = nullptr;

        uint32_t Size = 0;
        uint32_t ReservedSize = 0;
    };  
}