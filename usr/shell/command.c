#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "command.h"

static enum COMMANDS identifyCommand(char *cmdname)
{
    switch(cmdname[0])
    {
        case 'e':
        if(strcmp(cmdname + 1, "xit") == 0) return CMD_EXIT;
        break;

        case 'h':
        if(strcmp(cmdname + 1, "elp") == 0) return CMD_HELP;
        break;

        case 'r':
        if(strcmp(cmdname + 1, "un") == 0) return CMD_RUN;
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
}