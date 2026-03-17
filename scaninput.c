#include "headfile.h"

extern char prompt[25];
extern char input_string[25];
int pid = 0;
int status = 0;

S *head = NULL; // Singley linked list head node

void scan_input(char *prompt, char *input_string)
{
    signal(SIGINT, signal_handler);  // Ctrl+C signal registeration
    signal(SIGTSTP, signal_handler); // Ctrl+Z signal registeration

    extract_external_commands(external_commands); // Extract the external commands to the 2D array from file

    while (1)
    {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
        scanf("%[^\n]", input_string);
        getchar();
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            if (input_string[4] != ' ')
            {
                strcpy(prompt, input_string + 4);
            }
            else
            {
                printf("%s : command not found\n", input_string + 5);
            }
        }
        else
        {
            char *cmd = get_command(input_string);
            // printf("First word is %s\n",cmd);

            int type = check_command_type(cmd);
            // printf("return val %d\n",type);

            if (type == BUILTIN)
            {
                execute_internal_commands(input_string);
            }
            else if (type == EXTERNAL)
            {
                pid = fork();
                if (pid > 0)
                {
                    waitpid(pid, &status, WUNTRACED);
                }
                else if (pid == 0)
                {
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_command(input_string);
                }
            }
            else if (type == NO_COMMAND)
            {
                printf("%s : command not found\n", cmd);
            }
        }
    }
}

void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        if (pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
        }
    }
    else if (signum == SIGTSTP)
    {
        if (pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt);
        }
        else
        {
            insert_at_first(&head, pid, input_string);
        }
    }
    else if (signum == SIGCHLD)
    {
        waitpid(-1, &status, WNOHANG);
    }
}

int insert_at_first(S **head, int pid, char *cmd)
{
    S *new = malloc(sizeof(S));
    if (new == NULL)
        return 1;

    new->Pid = pid;
    strcpy(new->command, cmd);
    new->link = *head;
    *head = new;

    return 0;
}