#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *token;
    char *saveptr;
    int cmd_count = 0;

    // Initialize the command list
    memset(clist, 0, sizeof(command_list_t));

    // Make a copy of the input string to avoid modifying the original
    char cmd_line_copy[SH_CMD_MAX];
    strncpy(cmd_line_copy, cmd_line, SH_CMD_MAX - 1);
    cmd_line_copy[SH_CMD_MAX - 1] = '\0';

    // Count the number of commands (separated by '|')
    token = strtok_r(cmd_line_copy, PIPE_STRING, &saveptr);
    while (token != NULL) {
        cmd_count++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    // If the number of commands exceeds CMD_MAX, return an error
    if (cmd_count > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }

    // Reset cmd_count and re-parse the command line to populate clist
    cmd_count = 0;

    // Restore the original input
    strncpy(cmd_line_copy, cmd_line, SH_CMD_MAX - 1); 
    cmd_line_copy[SH_CMD_MAX - 1] = '\0';
    token = strtok_r(cmd_line_copy, PIPE_STRING, &saveptr);

    while (token != NULL && cmd_count < CMD_MAX) {
        // Remove leading and trailing spaces
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';

        // Split the command into executable and arguments
        char *exe = strtok(token, " ");
        if (exe != NULL) {
            strncpy(clist->commands[cmd_count].exe, exe, EXE_MAX - 1);
            clist->commands[cmd_count].exe[EXE_MAX - 1] = '\0';

            char *arg = strtok(NULL, "");
            if (arg != NULL) {
                strncpy(clist->commands[cmd_count].args, arg, ARG_MAX - 1);
                clist->commands[cmd_count].args[ARG_MAX - 1] = '\0';
            }
            else {
                clist->commands[cmd_count].args[0] = '\0';
            }

            cmd_count++;
        }

        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    if (cmd_count == 0) {
        return WARN_NO_CMDS;
    }

    clist->num = cmd_count;
    return OK;
}