
#include <Windows.h>

#include "file-system.h"

bool set_working_directory(char *path)
{
    return SetCurrentDirectory(path);
}
