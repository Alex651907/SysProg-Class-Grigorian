#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

void trim_spaces(char *str) {
    char *end;

    //Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') {
        return;
    }

    //Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    //Write null terminator
    *(end + 1) = '\0';
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    char *token;
    int argc = 0;
    bool in_quotes = false;
    char *start = cmd_line;

    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    trim_spaces(cmd_line);

    while (*start && argc < CMD_ARGV_MAX - 1) {
        while (*start && isspace((unsigned char)*start)) start++;

        if (*start == '\0') break;

        if (*start == '"') {
            in_quotes = true;
            start++;
            token = start;
            while (*start && (*start != '"' || in_quotes)) {
                if (*start == '\\' && *(start + 1) == '"') {
                    start++;
                }
                start++;
            }
            if (*start == '"') {
                *start = '\0';
                start++;
            }
        } else {
            token = start;
            while (*start && !isspace((unsigned char)*start)) start++;
            if (*start) {
                *start = '\0';
                start++;
            }
        }

        cmd_buff->argv[argc] = strdup(token);
        if (cmd_buff->argv[argc] == NULL) {
            return ERR_MEMORY;
        }
        argc++;
    }

    //Check for too many arguments
    if (*start) {
        fprintf(stderr, "error: too many arguments\n");
        return ERR_TOO_MANY_COMMANDS;
    }

    //Null-terminate the argv array
    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0) {
        return BI_NOT_BI;
    }

    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc == 1) {
            //No arguments, do nothing
            return BI_CMD_CD;
        } else if (cmd->argc == 2) {
            //Change directory
            if (chdir(cmd->argv[1]) == -1) {
                perror("cd");
                return BI_RC;
            }
            return BI_CMD_CD;
        } else {
            fprintf(stderr, "cd: too many arguments\n");
            return BI_RC;
        }
    }

    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return ERR_EXEC_CMD;
    } else if (pid == 0) {
        //Child process
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        //Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return ERR_EXEC_CMD;
    }
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *token;
    int num_commands = 0;
    char *saveptr;

    //Clear the command list
    memset(clist, 0, sizeof(command_list_t));

    //Split the command line by pipes
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    while (token != NULL) {
        trim_spaces(token);
        if (strlen(token) > 0) {
            if (num_commands >= CMD_MAX) {
                for (int i = 0; i < num_commands; i++) {
                    for (int j = 0; j < clist->commands[i].argc; j++) {
                        free(clist->commands[i].argv[j]);
                    }
                }
                fprintf(stderr, CMD_ERR_PIPE_LIMIT, CMD_MAX);
                return ERR_TOO_MANY_COMMANDS;
            }

            int rc = build_cmd_buff(token, &clist->commands[num_commands]);
            if (rc != OK) {
                for (int i = 0; i < num_commands; i++) {
                    for (int j = 0; j < clist->commands[i].argc; j++) {
                        free(clist->commands[i].argv[j]);
                    }
                }
                return rc;
            }
            num_commands++;
        }
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    if (num_commands == 0) {
        return WARN_NO_CMDS;
    }

    clist->num = num_commands;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;
    int pipefds[2 * (num_commands - 1)];
    pid_t pids[num_commands];
    int status;

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return ERR_EXEC_CMD;
        } else if (pids[i] == 0) {
            //Child process
            if (i != 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i != num_commands - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (num_commands - 1); j++) {
                close(pipefds[j]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 2 * (num_commands - 1); i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            return ERR_EXEC_CMD;
        }
    }

    return OK;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop() {
    char cmd_line[SH_CMD_MAX];
    command_list_t clist;

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        trim_spaces(cmd_line);

        //Check for empty input or input with only spaces
        if (strlen(cmd_line) == 0) {
            fprintf(stderr, CMD_WARN_NO_CMD);
            continue;
        }

        int rc = build_cmd_list(cmd_line, &clist);
        if (rc != OK) {
            if (rc == WARN_NO_CMDS) {
                fprintf(stderr, CMD_WARN_NO_CMD);
            } else if (rc == ERR_TOO_MANY_COMMANDS) {
                fprintf(stderr, CMD_ERR_PIPE_LIMIT, CMD_MAX);
            } else {
                fprintf(stderr, "Error: Failed to parse command.\n");
            }
            continue;
        }

        //Handle built-in commands or external commands
        if (clist.num == 1) {
            Built_In_Cmds bi_cmd = exec_built_in_cmd(&clist.commands[0]);
            if (bi_cmd == BI_CMD_EXIT) {
                break;
            } else if (bi_cmd == BI_CMD_CD) {
                continue;
            } else if (bi_cmd == BI_NOT_BI) {
                rc = exec_cmd(&clist.commands[0]);
                if (rc != OK) {
                    fprintf(stderr, "Error: Command execution failed.\n");
                }
            }
        } else {
            rc = execute_pipeline(&clist);
            if (rc != OK) {
                fprintf(stderr, "Error: Pipeline execution failed.\n");
            }
        }

        for (int i = 0; i < clist.num; i++) {
            for (int j = 0; j < clist.commands[i].argc; j++) {
                free(clist.commands[i].argv[j]);
            }
        }
    }

    return OK;
}