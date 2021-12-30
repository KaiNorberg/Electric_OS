#include "System.h"

#include "../String/cstr.h"
#include "../UserInput/Mouse.h"

namespace System
{
    struct Command
    {
        const char* Name;
        uint64_t Hash;
        bool (*Function)(const char*);

        Command(const char* Name, bool (*Function)(const char*))
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

        SettableInt(const char* Name, void* Variable, uint8_t VariableSize)
        {
            this->Name = Name;
            this->Variable = Variable;
            this->VariableSize = VariableSize;
        }
    };

    bool CommandSet(const char* Command)
    {
        SettableInt SettableInts[] =
        {
            SettableInt("mousedraw", &Mouse::Draw, sizeof(Mouse::Draw))
        };

        return false;
    }

    char SystemOutput[4096];
    const char* System(const char* Input)
    {    
        Command Commands[] =
        {
            Command("set", CommandSet)
        };

        uint64_t Hash = cstr::HashWord(Input);
        for (int i = 0; i < sizeof(Commands)/sizeof(Commands[0]); i++)
        {
            if (Hash == Commands[i].Hash)
            {
                return "Detected match!";
            }
        }

        return "ERROR: Command not found";
    }
}