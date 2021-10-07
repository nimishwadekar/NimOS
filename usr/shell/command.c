#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <sys/system.h>
#include <time.h>
#include <unistd.h>
#include "command.h"
#include "ui.h"

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
    char *cmdtok = strtok(cmd, " ");
    enum COMMANDS cmdtype = identifyCommand(cmdtok);

    switch(cmdtype)
    {
        case CMD_HELP:
        break;

        case CMD_RUN:
        cmdtok = strtok(NULL, " ");
        if(cmdtok == NULL)
        {
            printf("run: ");
            setfg(ERR_TEXT_COLOUR);
            printf("Fatal error: ");
            setfg(TEXT_COLOUR);
            printf("No executable file specified\n");
            break;
        }
        break;

        case CMD_SHUTDOWN:
        shutdown();
        break;

        case CMD_TIME:
        if((cmdtok = strtok(NULL, " ")) != NULL)
        {
            printf("%s: Invalid option for 'time'\n", cmdtok);
            break;
        }

        {
            static char time_wdayBuf[10];
            static char time_monBuf[10];

            time_t t = time(NULL);
            struct tm st = localtime(&t);
            printf("%d:%d:%d  %s, %d %s, %d\n", st.tm_hour, st.tm_min, st.tm_sec, 
                time_wday_str(&t, time_wdayBuf, 3), st.tm_mday, time_mon_str(&t, time_monBuf, 3), st.tm_year);
        }
        break;

        case CMD_INVALID:
        printf("%s: Unrecognized command\n", cmdtok);
        break;
    }
}