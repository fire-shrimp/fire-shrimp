#pragma once

#include <stdint.h>

typedef struct command_args {
    char *working_directory;
    uint32_t debug_port;
} command_args;

void set_default_args(command_args *args);

void parse_command_args(command_args *args, int argc, char **argv);
