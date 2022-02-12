#include "System.h"

#include "STL/String/cstr.h"
#include "STL/Process/Process.h"

#include "Programs/tty/tty.h"
#include "Programs/Desktop/Desktop.h"
#include "Programs/Topbar/Topbar.h"
#include "Programs/SystemMenu/SystemMenu.h"
#include "Programs/StartMenu/StartMenu.h"
#include "Programs/Terminal/Terminal.h"
#include "Programs/Calculator/Calculator.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/RTC/RTC.h"
#include "kernel/PIT/PIT.h"
#include "kernel/Debug/Debug.h"
#include "kernel/IO/IO.h"
#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Heap.h"
#include "kernel/ProcessHandler/ProcessHandler.h"
#include "kernel/ACPI/ACPI.h"
#include "kernel/PCI/PCI.h"

#include <cstdarg>

namespace System
{
    static char CommandOutput[1024];


    const char* CommandSet(const char* Command)
    {        
        const char* Variable = STL::NextWord(Command);
        const char* Value = STL::NextWord(Variable);

        struct SettableVar
        {
            const char* Name;
            uint64_t Hash;
            void* Ptr;
            uint64_t Size;

            SettableVar(const char* Name, void* Ptr, uint64_t Size)
            {
                this->Name = Name;
                this->Hash = STL::HashWord(Name);
                this->Ptr = Ptr;
                this->Size = Size;
            }
        };

        SettableVar SettableVars[] =
        {
            SettableVar("drawmouse", &Renderer::DrawMouse, sizeof(Renderer::DrawMouse))
        };

        uint64_t Hash = STL::HashWord(Variable);
        for (int i = 0; i < sizeof(SettableVars)/sizeof(SettableVars[0]); i++)
        {
            if (Hash == SettableVars[i].Hash)
            {
                switch (SettableVars[i].Size)
                {
                case 1:
                {
                    *(uint8_t*)SettableVars[i].Ptr = STL::ToInt(Value);
                }   
                break;
                case 2:
                {
                    *(uint16_t*)SettableVars[i].Ptr = STL::ToInt(Value);
                }   
                break;
                case 4:
                {
                    *(uint32_t*)SettableVars[i].Ptr = STL::ToInt(Value);
                }   
                break;
                case 8:
                {
                    *(uint64_t*)SettableVars[i].Ptr = STL::ToInt(Value);
                }   
                break;
                }
                
                return "Variable set";
            }
        }

        return "ERROR: Variable not found";
    }

    const char* CommandList(const char* Command)
    {         
        char* CurrentLocation = CommandOutput;
        char* LineStart = CommandOutput;

        auto Write = [&](const char* Input) 
        { 
            CurrentLocation = STL::CopyString(CurrentLocation, Input) + 1;
        }; 

        auto NextEntry = [&](const char* Input) 
        { 
            uint32_t Amount = 15 - ((uint64_t)CurrentLocation - (uint64_t)LineStart) % 15;
            for (int i = 0; i < Amount; i++)
            {
                Write(" ");            
            }    
            CurrentLocation = STL::CopyString(CurrentLocation, "| ") + 1;
            Write(Input);
        }; 

        auto StartLine = [&](const char* Input) 
        { 
            Write(" ");
            Write(Input);
        }; 
        
        auto NewLine = [&]() 
        {
            *CurrentLocation = '\n';
            CurrentLocation++;            
            *CurrentLocation = '\r';
            CurrentLocation++;
            LineStart = CurrentLocation;      
        };

        auto WriteLine = [&](uint64_t EntryAmount) 
        {            
            NewLine();
            for (uint32_t i = 0; i < EntryAmount; i++)
            {
                CurrentLocation = STL::CopyString(CurrentLocation, "+--------------") + 1;                
            }            
            *CurrentLocation = '+';
            CurrentLocation++;            
        };

        uint64_t Hash = STL::HashWord(STL::NextWord(Command));
        switch (Hash)
        {
        case STL::ConstHashWord("process"):
        {            
            StartLine("TITLE");
            NextEntry("ID");

            WriteLine(2);

            for (int i = 0; i < ProcessHandler::Processes.Length(); i++)
            {                
                NewLine();

                StartLine(ProcessHandler::Processes[i]->GetTitle());

                NextEntry(STL::ToString(ProcessHandler::Processes[i]->GetID()));
            }
        }
        break;
        case STL::ConstHashWord("pci"):
        {            
            StartLine("VENDOR");
            NextEntry("TYPE");

            WriteLine(2);
            
            SDTHeader* MCFG = ACPI::FindTable("MCFG");
        
            DeviceHeader* Device;
            PCI::ResetEnumeration();
            while (PCI::Enumerate(MCFG, Device))
            {                
                NewLine();

                StartLine(Device->GetVendorString());

                NextEntry(Device->GetTypeString());
            }    
        }
        break;
        default:
        {
            return "ERROR: List not found";
        }
        break;
        }

        *CurrentLocation = 0;
        return CommandOutput;
    }

    const char* CommandHelp(const char* Command)
    {              
        struct Manual
        {
            const char* Name;
            uint64_t Hash;
            const char* String;
            const char* Description;

            Manual(const char* Name, const char* Description, const char* String)
            {
                this->Name = Name;
                this->Hash = STL::HashWord(Name);
                this->String = String;
                this->Description = Description;
            }
        }; 

        Manual Manuals[] =
        {
            Manual("set", "An interface to access kernel variables.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    set - An interface to access kernel variables.\n\n\r"
            FOREGROUND_COLOR(086, 182, 194)"SYNOPSIS:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    set [VARIABLE] [VALUE]\n\n\r"
            FOREGROUND_COLOR(224, 108, 117)"    VARIABLE:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        drawmouse - A boolean value that sets if a cursor is drawn to the screen.\n\n\r"
            FOREGROUND_COLOR(086, 182, 194)"    VALUE:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        Any positive integer.\n\n\r"
            ),
            Manual("list", "An interface to list information about the kernel or hardware.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    list - An interface to list information about the kernel or hardware.\n\n\r"
            FOREGROUND_COLOR(086, 182, 194)"SYNOPSIS:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    list [LIST]\n\n\r"
            FOREGROUND_COLOR(224, 108, 117)"    LIST:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        process - A list of all currently running processes.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        pci - A list of all connected PCI devices.\n\r"
            ),
            Manual("time", "Allows access to the time values read from the appropriate CMOS registers.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    time - Allows access to the time values read from the appropriate CMOS registers.\n\r"
            ),
            Manual("date", "Allows access to the time values read from the appropriate CMOS registers.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    date - Allows access to the time values read from the appropriate CMOS registers.\n\r"
            ),           
            Manual("clear", "Send a request to clear the framebuffer of the process that called the command.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    clear - Send a request to clear the framebuffer of the process that called the command.\n\r"
            ),           
            Manual("start", "An interface to start any kernel level processes.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    start - An interface to start any kernel level processes.\n\n\r"
            FOREGROUND_COLOR(086, 182, 194)"SYNOPSIS:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    start [PROCESS]\n\n\r"
            FOREGROUND_COLOR(224, 108, 117)"    PROCESS:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        calculator - A calculator.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        desktop - The process responsible for drawing the desktop background.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        topbar - The process responsible for giving the user access to the startmenu and systemmenu.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        startmenu - The process used to start process when using the desktop.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        systemmenu - The process used to access the system when using the desktop.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        terminal - A GUI terminal.\n\r"
            FOREGROUND_COLOR(255, 255, 255)"        tty - A terminal like process that starts at boot.\n\r"
            ),           
            Manual("suicide", "Send a request to kill the process that called the command.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    suicide - Send a request to kill the process that called the command.\n\r"
            ),           
            Manual("restart", "Restarts the pc.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    restart - Restarts the pc.\n\r"
            ),           
            Manual("heapvis", "Shows a visualization of all the segments in the heap.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    heapvis - Shows a visualization of all the segments in the heap.\n\r"
            ),           
            Manual("sysfetch", "A neofetch lookalike to give system information.",
            FOREGROUND_COLOR(086, 182, 194)"\nNAME:\n\r"
            FOREGROUND_COLOR(255, 255, 255)"    sysfetch - A neofetch lookalike to give system information.\n\r"
            ), 
        };  

        uint64_t Hash = STL::HashWord(STL::NextWord(Command));
        if (Hash == 0)
        {
            char* CurrentLocation = CommandOutput;
            CurrentLocation = STL::CopyString(CurrentLocation, "\nType help [COMMAND] for information about the specified COMMAND.\n\n\r") + 1;
            CurrentLocation = STL::CopyString(CurrentLocation, FOREGROUND_COLOR(224, 108, 117)"COMMAND: \n\r"FOREGROUND_COLOR(255, 255, 255)) + 1;

            for (int i = 0; i < sizeof(Manuals)/sizeof(Manuals[0]); i++)
            {                
                CurrentLocation = STL::CopyString(CurrentLocation, "    ") + 1;
                CurrentLocation = STL::CopyString(CurrentLocation, Manuals[i].Name) + 1;
                CurrentLocation = STL::CopyString(CurrentLocation, " - ") + 1;
                CurrentLocation = STL::CopyString(CurrentLocation, Manuals[i].Description) + 1;
                CurrentLocation = STL::CopyString(CurrentLocation, "\r\n") + 1;
            }
            *CurrentLocation = 0;
            return CommandOutput;
        }

        for (int i = 0; i < sizeof(Manuals)/sizeof(Manuals[0]); i++)
        {   
            if (Manuals[i].Hash == Hash)
            {
                return Manuals[i].String;
            }
        }   

        return "ERROR: Invalid value of COMMAND ";

        /*return 
        FOREGROUND_COLOR(086, 182, 194)"set [VARIABLE] [VALUE]\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Sets the specified kernel variable to the specified value\n\r"
        FOREGROUND_COLOR(086, 182, 194)"list\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints the specified list\n\r"
        FOREGROUND_COLOR(086, 182, 194)"help\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints this menu\n\r"
        FOREGROUND_COLOR(086, 182, 194)"time\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints the current time\n\r"
        FOREGROUND_COLOR(086, 182, 194)"date\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints the current date\n\r"
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
        FOREGROUND_COLOR(086, 182, 194)"heapvis\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        Prints a visualization of all the segments of the heap\n\r"
        FOREGROUND_COLOR(086, 182, 194)"sysfetch\n\r"
        FOREGROUND_COLOR(224, 108, 117)"    DESC:\n\r"
        FOREGROUND_COLOR(255, 255, 255)"        A neofetch lookalike to give system information\n\r";*/
    }

    const char* CommandTime(const char* Command)
    {    
        uint64_t Hour = RTC::GetHour();
        uint64_t Minute = RTC::GetMinute();
        uint64_t Second = RTC::GetSecond();

        CommandOutput[0] = '0' + (Hour / 10); 
        CommandOutput[1] = '0' + (Hour % 10); 
        CommandOutput[2] = ':'; 
        CommandOutput[3] = '0' + (Minute / 10); 
        CommandOutput[4] = '0' + (Minute % 10); 
        CommandOutput[5] = ':'; 
        CommandOutput[6] = '0' + (Second / 10); 
        CommandOutput[7] = '0' + (Second % 10); 
        CommandOutput[8] = 0; 

        return CommandOutput;
    }

    const char* CommandDate(const char* Command)
    {            
        uint64_t Day = RTC::GetDay();
        uint64_t Month = RTC::GetMonth();
        uint64_t Year = RTC::GetYear();

        CommandOutput[0] = '0' + (Day / 10); 
        CommandOutput[1] = '0' + (Day % 10); 
        CommandOutput[2] = '/'; 
        CommandOutput[3] = '0' + (Month / 10); 
        CommandOutput[4] = '0' + (Month % 10); 
        CommandOutput[5] = '/'; 
        CommandOutput[6] = '2'; 
        CommandOutput[7] = '0'; 
        CommandOutput[8] = '0' + (Year / 10); 
        CommandOutput[9] = '0' + (Year % 10); 
        CommandOutput[10] = 0; 

        return CommandOutput;
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
        if (ProcessHandler::LastMessagedProcess != nullptr)
        {
            ProcessHandler::KillProcess(ProcessHandler::LastMessagedProcess->GetID());
        }
        return "";
    }

    const char* CommandClear(const char* Command)
    {
        if (ProcessHandler::LastMessagedProcess != nullptr)
        {
            ProcessHandler::LastMessagedProcess->Clear();
        }
        return "";
    }    

    const char* CommandShutdown(const char* Command)
    {

        return "";
    }  

    const char* CommandRestart(const char* Command)
    {
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
        struct StartableProcess
        {
            const char* Name;
            uint64_t Hash;
            STL::PROC Procedure;

            StartableProcess(const char* Name, STL::PROC Procedure)
            {
                this->Procedure = Procedure;
                this->Name = Name;
                this->Hash = STL::HashWord(Name);
            }
        };

        StartableProcess StartableProcesses[] =
        {
            StartableProcess("tty", tty::Procedure),
            StartableProcess("desktop", Desktop::Procedure),
            StartableProcess("topbar", Topbar::Procedure),
            StartableProcess("systemmenu", SystemMenu::Procedure),
            StartableProcess("startmenu", StartMenu::Procedure),
            StartableProcess("terminal", Terminal::Procedure),
            StartableProcess("calculator", Calculator::Procedure)
        };

        uint64_t Hash = STL::HashWord(STL::NextWord(Command));
        for (int i = 0; i < sizeof(StartableProcesses)/sizeof(StartableProcesses[0]); i++)
        {
            if (Hash == StartableProcesses[i].Hash)
            {
                return STL::ToString(ProcessHandler::StartProcess(StartableProcesses[i].Procedure));
            }
        }

        return "ERROR: Process not found";
    }

    const char* CommandHeapvis(const char* Command)
    {    
        char* Index = CommandOutput;
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

        return CommandOutput;
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

        Command Commands[] =
        {
            Command("set", CommandSet),
            Command("list", CommandList),
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