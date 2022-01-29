#include "System.h"

#include "STL/String/cstr.h"
#include "STL/Process/Process.h"

#include "Programs/tty/tty.h"
#include "Programs/Desktop/Desktop.h"
#include "Programs/Topbar/Topbar.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/RTC/RTC.h"
#include "kernel/PIT/PIT.h"
#include "kernel/Debug/Debug.h"
#include "kernel/IO/IO.h"
#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Heap.h"
#include "kernel/ProcessHandler/ProcessHandler.h"
#include "kernel/Input/KeyBoard.h"

#include <cstdarg>

namespace System
{
    struct Command
    {
        const char* Name;
        uint64_t Hash;
        const char* (*Function)(const char*);

        Command(const char* Name, const char* (*Function)(const char*))
        {
            this->Function = Function;
            this->Name = Name;
            this->Hash = STL::HashWord(Name);
        }
    };

    const char* CommandSet(const char* Command)
    {        
        const char* Variable = STL::NextWord(Command);
        const char* Value = STL::NextWord(Variable);

        switch (STL::HashWord(Variable))
        {
        case STL::ConstHashWord("drawmouse"):
        {
            Renderer::DrawMouse = STL::ToInt(Value);
        }
        break;
        default:
        {
            return "ERROR: Variable not found";
        }
        break;
        }

        return "Variable set";
    }
    
    const char* CommandHelp(const char* Command)
    {       
        return 
        FOREGROUND_COLOR(086, 182, 194)"set [VARIABLE] [VALUE]\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Sets the specified kernel variable to the specified value\n\r"
        FOREGROUND_COLOR(086, 182, 194)"help\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints this menu\n\r"
        FOREGROUND_COLOR(086, 182, 194)"time\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints the current time\n\r"
        FOREGROUND_COLOR(086, 182, 194)"date\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints the current date\n\r"
        FOREGROUND_COLOR(086, 182, 194)"kill\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Kills the process with the given ID\n\r"
        FOREGROUND_COLOR(086, 182, 194)"clear\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Clears the framebuffer of the process that performed the system call\n\r"
        FOREGROUND_COLOR(086, 182, 194)"start\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Starts the process with the given name\n\r"
        FOREGROUND_COLOR(086, 182, 194)"suicide\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Kills the process that performed the system call\n\r"
        FOREGROUND_COLOR(086, 182, 194)"restart\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Restarts the pc\n\r"
        FOREGROUND_COLOR(086, 182, 194)"shutdown\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Shuts down the pc\n\r"
        FOREGROUND_COLOR(086, 182, 194)"heapvis\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints a visualization of all the segments of the heap\n\r"
        FOREGROUND_COLOR(086, 182, 194)"sysfetch\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        A neofetch lookalike to give system information\n\r";
    }

    char CommandTimeOutput[64];
    const char* CommandTime(const char* Command)
    {
        char* Temp = CommandTimeOutput;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetHour())) + 1;
        Temp = STL::CopyString(Temp, ":") + 1;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetMinute())) + 1;
        Temp = STL::CopyString(Temp, ":") + 1;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetSecond())) + 1;
        *Temp = 0;
        return CommandTimeOutput;
    }

    char CommandDateOutput[64];
    const char* CommandDate(const char* Command)
    {
        char* Temp = CommandDateOutput;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetDay())) + 1;
        Temp = STL::CopyString(Temp, "/") + 1;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetMonth())) + 1;
        Temp = STL::CopyString(Temp, "/") + 1;
        Temp = STL::CopyString(Temp, "20") + 1;
        Temp = STL::CopyString(Temp, STL::ToString(RTC::GetYear())) + 1;
        *Temp = 0;
        return CommandDateOutput;
    }

    const char* CommandKill(const char* Command)
    {
        if (ProcessHandler::KillProcess(STL::ToInt(STL::NextWord(Command))))
        {
            return "Process killed";
        }
        else
        {
            return "ERROR: Could not kill process";
        }
    }

    const char* CommandSuicide(const char* Command)
    {
        if (ProcessHandler::LastMessagedProcess != 0)
        {
            ProcessHandler::KillProcess(ProcessHandler::LastMessagedProcess);
        }
        return "";
    }

    const char* CommandClear(const char* Command)
    {
        if (ProcessHandler::LastMessagedProcess != 0)
        {
            ProcessHandler::GetProcess(ProcessHandler::LastMessagedProcess)->Clear();
        }
        return "";
    }    

    const char* CommandShutdown(const char* Command)
    {

        return "";
    }  

    const char* CommandRestart(const char* Command)
    {
        asm("cli");

        ProcessHandler::KillAllProcesses();

        uint8_t Good = 0x02;
        while (Good & 0x02)
        {
            Good = IO::InByte(0x64);
        }
        IO::OutByte(0x64, 0xFE);

        while (true)
        {
            asm("HLT");
        }
        return "";
    }   

    const char* CommandStart(const char* Command)
    {
        switch (STL::HashWord(STL::NextWord(Command)))
        {
        case STL::ConstHashWord("tty"):
        {
            return STL::ToString(ProcessHandler::StartProcess(tty::Procedure));
        }
        break;
        case STL::ConstHashWord("desktop"):
        {
           return STL::ToString(ProcessHandler::StartProcess(Desktop::Procedure));
        }
        break;
        case STL::ConstHashWord("topbar"):
        {
            return STL::ToString(ProcessHandler::StartProcess(Topbar::Procedure));
        }
        break;
        default:
        {
            return "ERROR: Process not found";
        }
        break;
        }

        return "Process started";
    }

    char CommandHeapvisOutput[128];
    const char* CommandHeapvis(const char* Command)
    {
        char* Index = CommandHeapvisOutput;
        auto Write = [&](const char* String)
        {
            Index = STL::CopyString(Index, String) + 1;
            Index[0] = 0;
        };

        Write(FOREGROUND_COLOR(000, 000, 000));

        Heap::Segment* CurrentSegment = Heap::GetFirstSegment();
        while (true)
        {
            if (CurrentSegment->Free)
            {
                Write(BACKGROUND_COLOR(152, 195, 121));
            }
            else
            {
                Write(BACKGROUND_COLOR(224, 108, 117));
            }

            Write(" ");

            if (CurrentSegment->Size >= 1000)
            {
                Write(STL::ToString(CurrentSegment->Size / 1000));
                Write(" KB");  
            }
            else
            {
                Write(STL::ToString(CurrentSegment->Size));
                Write(" B");
            }

            Write(" ");
            Write(BACKGROUND_COLOR(000, 000, 000));
            Write(" ");

            if (CurrentSegment->Next == nullptr)
            {
                break;
            }
            CurrentSegment = CurrentSegment->Next;
        }

        Write(FOREGROUND_COLOR(255, 255, 255));
        Write(BACKGROUND_COLOR(000, 000, 000));

        return CommandHeapvisOutput;
    }

    const char* CommandSysfetch(const char* Command)
    {                
        const char* Sysfetch =                                                   
        FOREGROUND_COLOR(086, 182, 194)"         /ooooooooooo/                                                                                                      \n\r" 
        FOREGROUND_COLOR(086, 182, 194)"        /ooooooooooo/                                                                                                       \n\r"
        FOREGROUND_COLOR(086, 182, 194)"       /ooooooooooo/                                                                                                        \n\r"
        FOREGROUND_COLOR(086, 182, 194)"      /oooooooooo/                                                                                                          \n\r"
        FOREGROUND_COLOR(086, 182, 194)"     /oooooooooo/                                                                                                           \n\r"
        FOREGROUND_COLOR(086, 182, 194)"    /oooooooooo/                                                                                                            \n\r"
        FOREGROUND_COLOR(086, 182, 194)"   /oooooooooo/                                                                                                             \n\r"
        FOREGROUND_COLOR(086, 182, 194)"  /ooooooooooooooooooooo/                                                                                                   \n\r"
        FOREGROUND_COLOR(086, 182, 194)" /ooooooooooooooooooooo/                                                                                                    \n\r"
        FOREGROUND_COLOR(086, 182, 194)" /oooooooooooooooooooo/                                                                                                     \n\r"
        FOREGROUND_COLOR(086, 182, 194)"/ooooooooooooooooooo/                                                                                                       \n\r"
        FOREGROUND_COLOR(086, 182, 194)"           /ooooooo/                                                                                                        \n\r"
        FOREGROUND_COLOR(086, 182, 194)"          /oooooo/                                                                                                          \n\r"
        FOREGROUND_COLOR(086, 182, 194)"         /oooooo/                                                                                                           \n\r"
        FOREGROUND_COLOR(086, 182, 194)"         /oooo/                                                                                                             \n\r"
        FOREGROUND_COLOR(086, 182, 194)"        /oooo/                                                                                                              \n\r"
        FOREGROUND_COLOR(086, 182, 194)"       /oooo/                                                                                                               \n\r"
        FOREGROUND_COLOR(086, 182, 194)"      /ooo/                                                                                                                 \n\r"
        FOREGROUND_COLOR(255, 255, 255);

        auto Write = [&](uint64_t Line, const char* Part1, const char* Part2 = nullptr, const char* Part3 = nullptr) 
        { 
            char* Temp = (char*)((uint64_t)Sysfetch + Line * (11 + 126) + 11 + 27);
            Temp = STL::CopyString(Temp, Part1) + 1;
            if (Part2 != nullptr)
            {            
                Temp = STL::CopyString(Temp, FOREGROUND_COLOR(255, 255, 255)) + 1;
                Temp = STL::CopyString(Temp, Part2) + 1;
            }
            if (Part3 != nullptr)
            {            
                Temp = STL::CopyString(Temp, Part3) + 1;
            }
        }; 

        Write(1, "OS: ", OS_VERSION);
        Write(2, "Time: ", CommandTime(nullptr));
        Write(3, "Date: ", CommandDate(nullptr));
        Write(4, "Ticks: ", STL::ToString(PIT::Ticks));
        Write(5, "Uptime: ", STL::ToString(PIT::Ticks / PIT::GetFrequency()), " s   ");
        Write(6, "Free Heap: ", STL::ToString(Heap::GetFreeSize() / 1000), " KB   ");
        Write(7, "Used Heap: ", STL::ToString(Heap::GetUsedSize() / 1000), " KB   ");
        Write(8, "Total Heap: ", STL::ToString((Heap::GetUsedSize() + Heap::GetFreeSize()) / 1000), " KB   ");
        Write(9, "Heap Segments: ", STL::ToString(Heap::GetSegmentAmount()));
        Write(10, "Process Amount: ", STL::ToString(ProcessHandler::Processes.Length()));

        Write(14, "\033B040044052   \033B224108117   \033B229192123   \033B152195121   \033B097175239   \033B198120221   \033B000000000");
        Write(15, "\033B040044052   \033B224108117   \033B229192123   \033B152195121   \033B097175239   \033B198120221   \033B000000000");
         
        return Sysfetch;
    }   

    const char* System(const char* Input)
    {    
        Command Commands[] =
        {
            Command("set", CommandSet),
            Command("help", CommandHelp),
            Command("time", CommandTime),
            Command("date", CommandDate),
            Command("kill", CommandKill),
            Command("clear", CommandClear),
            Command("start", CommandStart),
            Command("restart", CommandRestart),
            Command("shutdown", CommandShutdown),
            Command("suicide", CommandSuicide),
            Command("heapvis", CommandHeapvis),
            Command("sysfetch", CommandSysfetch)
        };

        uint64_t Hash = STL::HashWord(Input);
        for (int i = 0; i < sizeof(Commands)/sizeof(Commands[0]); i++)
        {
            if (Hash == Commands[i].Hash)
            {
                return Commands[i].Function(Input);
            }
        }

        return "ERROR: Command not found";
    }

    STL::SYSRV Call(uint64_t Selector...)
    {
        va_list Args;
        va_start(Args, Selector);

        STL::SYSRV ReturnVal = 0;

        switch (Selector)
        {
        case 0:
        {
            ReturnVal = (STL::SYSRV)System::System(va_arg(Args, const char*));
        }
        break;
        case 1:
        {
            ReturnVal = (STL::SYSRV)Heap::Allocate(va_arg(Args, uint64_t));
        }
        break;
        case 2:
        {
            Heap::Free(va_arg(Args, void*));
        }
        break;
        }

        va_end(Args);

        return ReturnVal;
    }
}