#include "Debug.h"
#include "../Rendering/Renderer.h"
#include "../PIT/PIT.h"
#include "../String/cstr.h"
#include "../Memory/Pageing/PageAllocator.h"

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
        "...",
        "Greenpeace free'd the mallocs \\o/",
        "Typo in the code.",
        "System consumed all the paper for paging!",
        "I'm tired of this ;_;",
        "PC LOAD LETTER",
        "Abort, Retry, Fail?",
        "Et là c'est le drame...",
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
        Point StartPoint = Point(100, 100);

        Renderer::Clear(ARGB(255, 0, 0, 0));

        Renderer::CursorPos = StartPoint;
        Renderer::Print("KERNEL PANIC!\n\r", ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 1 * Scale);
        Renderer::Print("// ", ARGB(0), ARGB(255), Scale);
        Renderer::Print(ErrorJokes[PIT::Ticks % 20], ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 3 * Scale);
        Renderer::Print("Error returned message: ", ARGB(0), ARGB(255, 255, 0, 0), Scale);
        Renderer::Print(Message, ARGB(0), ARGB(255, 255, 0, 0), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 5 * Scale);
        Renderer::Print("Diagnostics: ", ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 6 * Scale);
        Renderer::Print("Ticks = ", ARGB(0), ARGB(255), Scale);
        Renderer::Print(cstr::ToString(PIT::Ticks), ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 8 * Scale);
        Renderer::Print("Memory: ", ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 9 * Scale);
        Renderer::Print("Total Memory = ", ARGB(0), ARGB(255), Scale);
        Renderer::Print(cstr::ToString(PageAllocator::GetTotalMemory()  / 1048576), ARGB(0), ARGB(255), Scale);
        Renderer::Print(" MB", ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 10 * Scale);
        Renderer::Print("Locked Memory = ", ARGB(0), ARGB(255), Scale);
        Renderer::Print(cstr::ToString(PageAllocator::GetLockedMemory()  / 1048576), ARGB(0), ARGB(255), Scale);
        Renderer::Print(" MB", ARGB(0), ARGB(255), Scale);

        Renderer::CursorPos = Point(StartPoint.X, StartPoint.Y + 16 * 11 * Scale);
        Renderer::Print("Free Memory = ", ARGB(0), ARGB(255), Scale);
        Renderer::Print(cstr::ToString(PageAllocator::GetFreeMemory()  / 1048576), ARGB(0), ARGB(255), Scale);
        Renderer::Print(" MB", ARGB(0), ARGB(255), Scale);

        while (true)
        {
            asm("HLT");
        }
    }
}