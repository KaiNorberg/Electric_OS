#include "System.h"

#define OS_VERSION "Electric_OS 30/12/2021"

#include "../String/cstr.h"
#include "../UserInput/Mouse.h"
#include "../RTC/RTC.h"
#include "../PIT/PIT.h"
#include "../Debug/Debug.h"
#include "../Memory/Pageing/PageAllocator.h"

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
            this->Hash = cstr::HashWord(Name);
        }
    };

    struct SettableInt
    {
        const char* Name;
        uint64_t Hash;

        void* Variable;
        uint8_t VariableSize;

        void SetVariable(uint64_t Value)
        {
            switch(VariableSize)
            {
            case 1:
            {
                *((uint8_t*)Variable) = Value;
            }
            break;
            case 2:
            {
                *((uint16_t*)Variable) = Value;
            }
            break;
            case 4:
            {
                *((uint32_t*)Variable) = Value;
            }
            break;
            case 8:
            {
                *((uint64_t*)Variable) = Value;
            }
            break;
            }
        }

        SettableInt(const char* Name, void* Variable, uint8_t VariableSize)
        {
            this->Name = Name;
            this->Hash = cstr::HashWord(Name);
            this->Variable = Variable;
            this->VariableSize = VariableSize;
        }
    };

    const char* CommandSet(const char* Command)
    {
        SettableInt SettableInts[] =
        {
            SettableInt("mousedraw", &Mouse::Draw, sizeof(Mouse::Draw))
        };

        char* Variable = cstr::NextWord(Command);
        char* Value = cstr::NextWord(Variable);

        uint64_t VariableHash = cstr::HashWord(Variable);

        for (int i = 0; i < sizeof(SettableInts)/sizeof(SettableInts[0]); i++)
        {
            if (VariableHash == SettableInts[i].Hash)
            {
                SettableInts[i].SetVariable(cstr::ToInt(Value));
                return "Variable set";
            }
        }

        return "ERROR: Variable not found";
    }

    char CommandTimeOutput[64];
    const char* CommandTime(const char* Command)
    {
        char* Temp = CommandTimeOutput;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetHour())) + 1;
        Temp = cstr::Copy(Temp, ":") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetMinute())) + 1;
        Temp = cstr::Copy(Temp, ":") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetSecond())) + 1;
        Temp[1] = 0;
        return CommandTimeOutput;
    }

    char CommandDateOutput[64];
    const char* CommandDate(const char* Command)
    {
        char* Temp = CommandDateOutput;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetDay())) + 1;
        Temp = cstr::Copy(Temp, "/") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetMonth())) + 1;
        Temp = cstr::Copy(Temp, "/") + 1;
        Temp = cstr::Copy(Temp, "20") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString(RTC::GetYear())) + 1;
        Temp[1] = 0;
        return CommandDateOutput;
    }

    const char* CommandPanic(const char* Command)
    {
        Debug::Error("Test!");
        return nullptr;
    }

    char CommandSysfetchOutput[1100];
    const char* CommandSysfetch(const char* Command)
    {                                    
        char* Temp = CommandSysfetchOutput;          

        Temp = cstr::Copy(Temp, "\033F086182194         @@@@@@@@@@@@@    ") + 1;
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;

        Temp = cstr::Copy(Temp, "        @@@@@@@@@@@@@     ") + 1;
        Temp = cstr::Copy(Temp, "OS\033F255255255: ") + 1;
        Temp = cstr::Copy(Temp, OS_VERSION) + 1;
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;    

        Temp = cstr::Copy(Temp, "       @@@@@@@@@@@@@      ") + 1;
        Temp = cstr::Copy(Temp, "Uptime\033F255255255: ") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString(PIT::Ticks / PIT::GetFrequency())) + 1;
        Temp = cstr::Copy(Temp, " s") + 1;
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1; 

        Temp = cstr::Copy(Temp, "      @@@@@@@@@@@@        ") + 1; 
        Temp = cstr::Copy(Temp, "Time\033F255255255: ") + 1;  
        Temp = cstr::Copy(Temp, CommandTime(nullptr)) + 1;  
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1; 

        Temp = cstr::Copy(Temp, "     @@@@@@@@@@@@         ") + 1;  
        Temp = cstr::Copy(Temp, "Date\033F255255255: ") + 1;  
        Temp = cstr::Copy(Temp, CommandDate(nullptr)) + 1;  
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;    

        Temp = cstr::Copy(Temp, "    @@@@@@@@@@@@          ") + 1;  
        Temp = cstr::Copy(Temp, "Memory\033F255255255: ") + 1;

        Temp = cstr::Copy(Temp, cstr::ToString(PageAllocator::GetLockedMemory() / 1048576)) + 1;
        Temp = cstr::Copy(Temp, " / ") + 1;
        Temp = cstr::Copy(Temp, cstr::ToString((PageAllocator::GetFreeMemory() + PageAllocator::GetLockedMemory()) / 1048576)) + 1;
        Temp = cstr::Copy(Temp, " MB") + 1; 
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;  

        Temp = cstr::Copy(Temp, "   @@@@@@@@@@@@           ") + 1; 
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;   

        Temp = cstr::Copy(Temp, "  @@@@@@@@@@@@@@@@@@@@@@@ ") + 1;   
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;    

        Temp = cstr::Copy(Temp, " @@@@@@@@@@@@@@@@@@@@@@@  ") + 1; 
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;    

        Temp = cstr::Copy(Temp, " @@@@@@@@@@@@@@@@@@@@@@   ") + 1; 
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;   

        Temp = cstr::Copy(Temp, "@@@@@@@@@@@@@@@@@@@@@     ") + 1; 
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;   

        Temp = cstr::Copy(Temp, "           @@@@@@@@@      ") + 1;  
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;   

        Temp = cstr::Copy(Temp, "          @@@@@@@@        ") + 1;  
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1; 

        Temp = cstr::Copy(Temp, "         @@@@@@@@         ") + 1;  
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;  

        Temp = cstr::Copy(Temp, "         @@@@@@           ") + 1;  
        Temp = cstr::Copy(Temp, "\033B040044052   \033B224108117   \033B229192123   \033B152195121   \033B097175239   \033B198120221   \033B086182194   \033B220223228   ") + 1;   
        Temp = cstr::Copy(Temp, "\n\r\033F086182194\033B000000000") + 1;   

        Temp = cstr::Copy(Temp, "        @@@@@@            ") + 1;   
        Temp = cstr::Copy(Temp, "\033B040044052   \033B224108117   \033B229192123   \033B152195121   \033B097175239   \033B198120221   \033B086182194   \033B220223228   ") + 1;   
        Temp = cstr::Copy(Temp, "\n\r\033F086182194\033B000000000") + 1;

        Temp = cstr::Copy(Temp, "       @@@@@@             ") + 1;   
        Temp = cstr::Copy(Temp, "\n\r\033F086182194") + 1;

        Temp = cstr::Copy(Temp, "      @@@@@               \033F255255255") + 1;

        Temp[1] = 0;                   
        return CommandSysfetchOutput;
    }                                                                                                                                                            

    const char* System(const char* Input)
    {    
        Command Commands[] =
        {
            Command("set", CommandSet),
            Command("time", CommandTime),
            Command("date", CommandDate),
            Command("panic", CommandPanic),
            Command("sysfetch", CommandSysfetch)
        };

        uint64_t Hash = cstr::HashWord(Input);
        for (int i = 0; i < sizeof(Commands)/sizeof(Commands[0]); i++)
        {
            if (Hash == Commands[i].Hash)
            {
                return Commands[i].Function(Input);
            }
        }

        return "ERROR: Command not found";
    }

    /*const char* System(const char* Input)
    {
        return Input;
    }*/
}