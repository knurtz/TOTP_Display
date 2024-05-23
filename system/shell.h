#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include "pico/stdlib.h"

void Shell_Init(void);

// Check receive buffer for newline, returns 1 if command was found
uint8_t Shell_CheckCommand(void);

#endif /* SHELL_H */