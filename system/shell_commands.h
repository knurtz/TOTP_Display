#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#include <stdio.h>
#include "pico/stdlib.h"

typedef struct shell_command
{
  const char command[16];
  void (*callback)(int arglen, char* argv);
} ShellCommand;

extern ShellCommand command_set[];
extern size_t command_cnt;

#endif /* SHELL_COMMANDS_H */