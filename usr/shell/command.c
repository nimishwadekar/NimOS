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
        case 'c':
        if(strcmp(cmdname + 1, "ls") == 0) return CMD_CLS;
        break;

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

void printError(char *cmd, int fatal, char *errmsg)
{
    printf("%s: ", cmd);
    setfg(ERR_TEXT_COLOUR);
    printf("%s: ", (fatal ? "Fatal error" : "Error"));
    setfg(TEXT_COLOUR);
    printf("%s\n", errmsg);
}


void runCommand(char *cmd)
{
    char *cmdtok = strtok(cmd, " ");
    enum COMMANDS cmdtype = identifyCommand(cmdtok);

    switch(cmdtype)
    {
        /*********************************************************/
        case CMD_CLS:
        setbg(getbg());
        printLogo();
        break;

        /*********************************************************/
        case CMD_HELP:
        cmdtok = strtok(NULL, " ");
        if(cmdtok == NULL)
        {
            printError("help", 0, "No command specified");
            break;
        }
        {
            char *helpcmd = cmdtok;
            cmdtok = strtok(NULL, " ");
            if(cmdtok != NULL)
            {
                printError("help", 0, "More than one command specified");
                break;
            }

            switch(identifyCommand(helpcmd))
            {
                case CMD_CLS:
                printf("\nClears the screen and resets the text cursor to the beginning\nFormat:\n");
                printf("        cls\n\n");
                break;

                case CMD_HELP:
                printf("Displays brief information about a command\nFormat:\n");
                printf("        help    <command>\n\n");
                break;

                case CMD_RUN:
                printf("\nRuns an executable file\nFormat:\n");
                printf("        run     [-e]    <file>\n\nOptions:\n");
                printf("        -e      Displays exit code after program has terminated\n\n");
                break;

                case CMD_SHUTDOWN:
                printf("\nShuts down the computer\nFormat:\n");
                printf("        shutdown\n\n");
                break;

                case CMD_TIME:
                printf("\nDisplays the current time in the format Hour:Minute:Second  Weekday, Day Month, Year\nFormat:\n");
                printf("        time\n\n");
                break;

                case CMD_INVALID:
                printError("help", 0, "Unrecognized command");
                break;
            }
        }
        break;

        /*********************************************************/
        case CMD_RUN:
        {
            static char *argv[256];
            int showexit = 0;

            cmdtok = strtok(NULL, " ");
            if(cmdtok == NULL)
            {
                printError("run", 1, "No executable specified");
                break;
            }
            if(strcmp(cmdtok, "-e") == 0)
            {
                showexit = 1;
                cmdtok = strtok(NULL, " ");
                if(cmdtok == NULL)
                {
                    printError("run", 1, "No executable specified");
                    break;
                }
            }

            argv[0] = cmdtok;

            int argc = 1;
            while((cmdtok = strtok(NULL, " ")) != NULL)
                argv[argc++] = cmdtok;
            argv[argc] = NULL;

            int exitcode;
            int spawnresult = spawnv(argv[0], argv, &exitcode);
            if(spawnresult == -1)
            {
                printError("run", 0, "Could not open specified executable");
                break;
            }
            if(spawnresult == -2)
            {
                printError("run", 0, "Executable read failed");
                break;
            }
            
            if(showexit) printf("Process exited with exit code %d\n", exitcode);
        }
        break;

        /*********************************************************/
        case CMD_SHUTDOWN:
        shutdown();
        break;

        /*********************************************************/
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
            printf("%2d:%2d:%2d  %s, %2d %s, %4d\n", st.tm_hour, st.tm_min, st.tm_sec, 
                time_wday_str(&t, time_wdayBuf, 3), st.tm_mday, time_mon_str(&t, time_monBuf, 3), st.tm_year);
        }
        break;

        /*********************************************************/
        case CMD_INVALID:
        printf("%s: Unrecognized command\n", cmdtok);
        break;
    }
}