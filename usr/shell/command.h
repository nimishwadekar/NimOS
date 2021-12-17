#ifndef _COMMAND_H
#define _COMMAND_H 1

enum COMMANDS
{
    CMD_INVALID = -1,
    CMD_CLS,
    CMD_HELP,
    CMD_RUN,
    CMD_SHUTDOWN,
    CMD_TIME
};

void runCommand(char *cmd);

#endif