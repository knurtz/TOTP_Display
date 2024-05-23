#include "shell.h"

#include "shell_commands.h"

#include <stdlib.h>
#include <string.h>

char rec_buffer[100];
size_t buffer_len = 0;

static void Shell_ClearInputBuffer(void)
{
  memset(rec_buffer, 0, sizeof(rec_buffer));
  buffer_len = 0;
}

static void Shell_Overflow(void)
{
  printf("Buffer overflow\n");
  Shell_Init();
}

static void Shell_ExecuteCommand(void)
{
  if (strlen(rec_buffer) == 0) return;
  int command_length;

  for (int i = 0; i < command_cnt; i++)
  {
    if (strstr(rec_buffer, command_set[i].command))
    {
      command_length = strlen(command_set[i].command) + 1;
      command_set[i].callback(strlen(rec_buffer) - command_length, rec_buffer + command_length);
      return;
    }
  }

  printf("Unknown command.\n");
}

void Shell_Init(void)
{
  Shell_ClearInputBuffer();
  printf("\nTOTP> ");
}

// Check receive buffer for newline, indicating the end of a command
uint8_t Shell_CheckCommand(void)
{
  // Receive from USB
  int temp;
  while ((temp = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT)
  {
    *(rec_buffer + buffer_len++) = temp;
    if (buffer_len >= sizeof(rec_buffer)) Shell_Overflow();
  }

  // check for end of line
  char* newline_pos = strchr(rec_buffer, '\n');
  if (newline_pos)
  {
    printf("%s", rec_buffer);
    *newline_pos = '\0';
    Shell_ExecuteCommand();
    Shell_Init();
    return 1;
  }

  return 0;
}