

#include "command.h"
#include "config.h"
#include "string-util.h"

void set_default_args(command_args *args)
{
    args->working_directory = NULL;
    args->debug_port = DEFAULT_DEBUG_PORT;
}

void parse_command_args(command_args *args, int argc, char **argv)
{
    for(uint32_t i = 1; i < (uint32_t)argc; i++) {
        if(starts_with(argv[i], "-wd=", 4) && length(argv[i], 5) >= 5){
            args->working_directory = &argv[i][4];
        }
    }
}
