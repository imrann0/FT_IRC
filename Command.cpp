#include "Include/Command.hpp"
#include <cstring> // Strncmp

void routeCommand(Server *server, const char* buffer)
{
    if (strncmp(buffer, "NICK", 4) == 0)
    {

    }
}

