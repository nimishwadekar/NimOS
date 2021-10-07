#ifndef _COMMAND_H
#define _COMMAND_H 1

enum COMMANDS
{
    CMD_INVALID = -1,
    CMD_RUN,
    CMD_HELP,
    CMD_TIME,
    CMD_EXIT
};

void runCommand(char *cmd);

#endif