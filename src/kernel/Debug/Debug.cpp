#include "Debug.h"

#include "STL/String/cstr.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/PIT/PIT.h"
#include "kernel/RTC/RTC.h"
#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Heap.h"
#include "kernel/System/System.h"

namespace Debug
{
    const char* ErrorJokes[] = 
    {
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!",
        "Witty comment unavailable :(",
        "Surprise! Haha. Well, this is awkward.",
        "Oh - I know what I did wrong!",
        "Uh... Did I do that?",
        "Oops.",
        "DON'T PANIC!",
        "kernel.",
        "Greenpeace free'd the mallocs \\o/",
        "Typo in the code.",
        "System consumed all the paper for paging!",
        "I'm tired of this ;_;",
        "PC LOAD LETTER",
        "Abort, Retry, Fail?",
        "Everything's going to plan. No, really, that was supposed to happen.",
        "My bad.",
        "Quite honestly, I wouldn't worry myself about that.",
        "This doesn't make any sense!",
        "It's not a good surprise...",
        "Don't do that.",
        "Layer 8 problem detected."
        "PEBCAC detected."        
    };

    void Error(const char* Message)
    {
        asm("CLI");

        uint8_t Scale = 3;
        STL::Point StartPoint = STL::Point(100, 50);

        Renderer::Clear();

        Renderer::Background = STL::ARGB(0);
        Renderer::Foreground = STL::ARGB(255);

        Renderer::CursorPos = StartPoint;
        Renderer::Print("KERNEL PANIC!\n\r", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 1 * Scale);
        Renderer::Print("// ", Scale);
        Renderer::Print(ErrorJokes[PIT::Ticks % 20], Scale);

        Renderer::Background = STL::ARGB(0);
        Renderer::Foreground = STL::ARGB(255, 255, 0, 0);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 3 * Scale);
        Renderer::Print("ERROR: ", Scale);
        Renderer::Print("\"", Scale);
        Renderer::Print(Message, Scale);
        Renderer::Print("\"", Scale);

        Renderer::Background = STL::ARGB(0);
        Renderer::Foreground = STL::ARGB(255);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 5 * Scale);
        Renderer::Print("OS_VERSION = ", Scale);
        Renderer::Print(OS_VERSION, Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 7 * Scale);
        Renderer::Print("Time: ", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 8 * Scale);
        Renderer::Print("Ticks = ", Scale);
        Renderer::Print(STL::ToString(PIT::Ticks), Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 9 * Scale);
        Renderer::Print("Current Time = ", Scale);
        Renderer::Print(STL::ToString(RTC::GetHour()), Scale);
        Renderer::Print(":", Scale);
        Renderer::Print(STL::ToString(RTC::GetMinute()), Scale);
        Renderer::Print(":", Scale);
        Renderer::Print(STL::ToString(RTC::GetSecond()), Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 11 * Scale);
        Renderer::Print("Memory: ", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 12 * Scale);
        Renderer::Print("Used Heap = ", Scale);
        Renderer::Print(STL::ToString(Heap::GetUsedSize() / 1000), Scale);
        Renderer::Print(" KB", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 13 * Scale);
        Renderer::Print("Free Heap = ", Scale);
        Renderer::Print(STL::ToString(Heap::GetFreeSize() / 1000), Scale);
        Renderer::Print(" KB", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 14 * Scale);
        Renderer::Print("Locked Pages = ", Scale);
        Renderer::Print(STL::ToString(PageAllocator::GetLockedMemory() / 4096), Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 15 * Scale);
        Renderer::Print("Free Pages = ", Scale);
        Renderer::Print(STL::ToString(PageAllocator::GetFreeMemory() / 4096), Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 17 * Scale);
        Renderer::Print("System Halted!", Scale);

        Renderer::CursorPos = STL::Point(StartPoint.X, StartPoint.Y + 16 * 18 * Scale);
        Renderer::Print("Please manually reboot your machine.", Scale);

	    Renderer::SwapBuffers();

        while (true)
        {
            asm("HLT");
        }
    }
}