#pragma once

namespace Debug
{    
    /// <summary>
    /// Halts the system and prints the given message.
    /// After this function has been called the only way to leave is to reboot the pc.
    /// </summary>
    void Error(const char* Message);
}
