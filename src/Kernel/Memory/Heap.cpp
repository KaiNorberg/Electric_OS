#include "Heap.h"

#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Paging/PageTable.h"

namespace Heap
{
    Segment* FirstSegment = nullptr;
    Segment* LastSegment = nullptr;

    void* Segment::GetStart()
    {
        return (void*)((uint64_t)this + sizeof(Segment));
    }

    void* Segment::GetEnd()
    {
        return (void*)((uint64_t)this + sizeof(Segment) + this->Size);
    }

    void Segment::Split(uint64_t NewSize)
    {
        //TODO: MAKE THIS WORK

        /*if (this->Size < 64)
        {
            return; 
        }

        LastSegment->Next = (Segment*)((uint64_t)this->GetStart() + NewSize);
        LastSegment->Next->Size = (uint64_t)this->GetEnd() - ((uint64_t)this->GetStart() + NewSize + sizeof(Segment));
        LastSegment->Next->Free = true;
        LastSegment = LastSegment->Next;

        this->Size = NewSize;*/
    }

    void Init()
    {
        FirstSegment = (Segment*)HEAP_START;
        void* Address = FirstSegment;
        for (uint64_t i = 0; i < (HEAP_STARTSIZE + sizeof(Segment)) / 4096 + 1; i++)
        {
            PageTableManager::MapAddress(Address, PageAllocator::RequestPage());
            Address = (void*)((uint64_t)Address + 4096);
        }

        FirstSegment->Size = HEAP_STARTSIZE;
        FirstSegment->Next = nullptr;
        FirstSegment->Free = true;
        
        LastSegment = FirstSegment;
    }

    uint64_t GetUsedSize()
    {
        uint64_t UsedSize = 0;

        Segment* CurrentSegment = FirstSegment;
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

        Segment* CurrentSegment = FirstSegment;
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

    uint64_t GetSegmentAmount()
    {
        uint64_t SegmentAmount = 0;

        Segment* CurrentSegment = FirstSegment;
        while (true)
        {
            SegmentAmount++;

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        return SegmentAmount;
    }

    void* Allocate(uint64_t Size)
    {
        if (Size == 0)
        {
            return nullptr;
        }

        Size = Size + (64 - (Size % 64));

        Segment* CurrentSegment = FirstSegment;
        while (true)
        {
            if (CurrentSegment->Free)
            {
                if (CurrentSegment->Size == Size)
                {
                    CurrentSegment->Free = false;
                    return CurrentSegment->GetStart();
                }
                else if (CurrentSegment->Size > Size)
                {
                    CurrentSegment->Split(Size);
                    CurrentSegment->Free = false;
                    return CurrentSegment->GetStart();
                }
            }

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        Reserve(Size);
        LastSegment->Free = false;
        return LastSegment->GetStart();
    }

    void Free(void* Address)
    {
        Segment* segment = (Segment*)((uint64_t)Address - sizeof(Segment));
        segment->Free = true;
    }

    void Reserve(uint64_t Size)
    {   
        Size = Size + (4096 - (Size % 4096));
        Segment* NewSegment = (Segment*)LastSegment->GetEnd();

        void* Address = NewSegment;
        for (uint64_t i = 0; i < (Size + sizeof(Segment)) / 4096 + 1; i++)
        {
            PageTableManager::MapAddress(Address, PageAllocator::RequestPage());
            Address = (void*)((uint64_t)Address + 4096);
        }

        NewSegment->Size = Size;
        NewSegment->Next = nullptr;
        NewSegment->Free = true;

        LastSegment->Next = NewSegment;
        
        LastSegment = NewSegment;
    }
}