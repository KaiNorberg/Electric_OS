#include "Heap.h"

#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Paging/PageTable.h"

#define HEAP_START 0x100000000000
#define HEAP_STARTSIZE 0x10 * 4096

namespace Heap
{
    void* End = nullptr;
    Segment* LastSegment = nullptr;

    Segment* Segment::Split(uint64_t SplitSize)
    {
        if (Size < 64)
        {
            return nullptr; 
        }

        int64_t NewSize = this->Size - SplitSize - sizeof(Segment);
        if (NewSize < 64)
        {
            return nullptr;
        }

        Segment* NewSegment = (Segment*)((uint64_t)this + sizeof(Segment) + SplitSize);
        this->Next = NewSegment;
        this->Size = SplitSize;
        NewSegment->Next = nullptr;
        NewSegment->Size = NewSize;
        NewSegment->Free = this->Free;
        LastSegment = NewSegment;

        return NewSegment;
    }

    void Init()
    {
        End = (void*)HEAP_START;
        Reserve(HEAP_STARTSIZE);
    }

    uint64_t GetUsedSize()
    {
        uint64_t UsedSize = 0;

        Segment* CurrentSegment = (Segment*)HEAP_START;
        while (true)
        {
            if (!CurrentSegment->Free)
            {
                UsedSize += CurrentSegment->Size;
            }

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        return UsedSize;
    }

    uint64_t GetFreeSize()
    {
        uint64_t FreeSize = 0;

        Segment* CurrentSegment = (Segment*)HEAP_START;
        while (true)
        {
            if (CurrentSegment->Free)
            {
                FreeSize += CurrentSegment->Size;
            }

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        return FreeSize;
    }

    void* Allocate(uint64_t Size)
    {
        if (Size == 0)
        {
            return nullptr;
        }

        Size = Size + (64 - (Size % 64));

        Segment* CurrentSegment = (Segment*)HEAP_START;
        while (true)
        {
            if (CurrentSegment->Free)
            {
                if (CurrentSegment->Size == Size)
                {
                    CurrentSegment->Free = false;
                    return (void*)((uint64_t)CurrentSegment + sizeof(Segment));
                }
                else if (CurrentSegment->Size > Size)
                {
                    CurrentSegment->Split(Size);
                    CurrentSegment->Free = false;
                    return (void*)((uint64_t)CurrentSegment + sizeof(Segment));
                }
            }

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        Reserve(Size);
        return Allocate(Size);
    }

    void Free(void* Address)
    {
        Segment* segment = (Segment*)Address - 1;
        segment->Free = true;
    }

    void Reserve(uint64_t Size)
    {   
        Size = Size + (4096 - (Size % 4096));
        Segment* NewSegment = (Segment*)End;

        for (uint64_t i = 0; i < Size / 4096; i++)
        {
            PageTableManager::MapAddress(End, PageAllocator::RequestPage());
            End = (void*)((uint64_t)End + 4096);
        }

        NewSegment->Size = Size - sizeof(Segment);
        NewSegment->Next = nullptr;
        NewSegment->Free = true;

        if (LastSegment != nullptr)
        {
            LastSegment->Next = NewSegment;
        }
        
        LastSegment = NewSegment;
    }
}