#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "command.h"

static enum COMMANDS identifyCommand(char *cmdname)
{
    switch(cmdname[0])
    {
        case 'h':
        if(strcmp(cmdname + 1, "elp") == 0) return CMD_HELP;
        break;

        case 'r':
        if(strcmp(cmdname + 1, "un") == 0) return CMD_RUN;
        break;

        case 's':
        if(strcmp(cmdname + 1, "hutdown") == 0) return CMD_SHUTDOWN;
        break;

        case 't':
        if(strcmp(cmdname + 1, "ime") == 0) return CMD_TIME;
        break;

    }
    return CMD_INVALID;
}

void runCommand(char *cmd)
{
    char *tok = strtok(cmd, " ");
    enum COMMANDS cmdtype = identifyCommand(tok);

    switch(cmdtype)
    {
        case CMD_HELP:
        break;

        case CMD_RUN:
        break;

        case CMD_SHUTDOWN:
        break;

        case CMD_TIME:
        break;

        case CMD_INVALID:
        printf("%s: Unrecognized command\n", tok);
        break;
    }
}