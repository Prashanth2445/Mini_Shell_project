#include "headfile.h"

extern int status;
extern S *head;
char *get_command(char *input_string)
{
    static char command[20];
    int i = 0;

    while (input_string[i] != ' ' && input_string[i] != '\n' && input_string[i] != '\0')
    {
        command[i] = input_string[i];
        i++;
    }
    command[i] = '\0';
    return command;
}

int check_command_type(char *cmd)
{
    for (int i = 0; builtins[i]; i++)
    {
        if (strcmp(builtins[i], cmd) == 0)
        {
            // printf("--> %s\n", builtins[i]);
            return BUILTIN;
        }
    }
    for (int i = 0; external_commands[i]; i++)
    {
        if (strcmp(external_commands[i], cmd) == 0)
        {
            // printf("--> %s\n", external_commands[i]);
            return EXTERNAL;
        }
    }
    return NO_COMMAND;
}

void extract_external_commands(char **external_commands)
{
    int fd = open("external_cmd.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return;
    }

    char ch;
    char buffer[100];
    int i = 0, j = 0;

    while (read(fd, &ch, 1) > 0 && j < 151)
    {
        if (ch == '\n' || ch == '\r')
        {
            buffer[i] = '\0';

            /* remove last \r if present */
            int len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\r')
                buffer[len - 1] = '\0';

            /* store only if not empty */
            if (strlen(buffer) > 0)
            {
                external_commands[j] = malloc(strlen(buffer) + 1);
                strcpy(external_commands[j], buffer);
                j++;
            }

            i = 0;
        }
        else
        {
            if (i < 99)
                buffer[i++] = ch;
        }
    }

    /* handle last line if no newline at EOF */
    if (i > 0 && j < 151)
    {
        buffer[i] = '\0';

        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\r')
            buffer[len - 1] = '\0';

        if (strlen(buffer) > 0)
        {
            external_commands[j] = malloc(strlen(buffer) + 1);
            strcpy(external_commands[j], buffer);
            j++;
        }
    }

    external_commands[j] = NULL;
    close(fd);
}

void execute_internal_commands(char *input_string)
{
    if (strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(input_string, "pwd") == 0)
    {
        char buff[50];
        getcwd(buff, 50);
        printf("%s\n", buff);
    }
    else if (strncmp(input_string, "cd", 2) == 0)
    {
        char buff[50];
        chdir(input_string + 3);
        getcwd(buff, 50);
        printf("%s\n", buff);
    }
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("%d", getpid());
    }
    else if (strcmp(input_string, "echo $?") == 0)
    {
        if (WIFEXITED(status))
        {
            printf("%d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("0\n");
        }
    }
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        printf("%s\n", getenv("SHELL"));
    }
    else if (strcmp(input_string, "jobs") == 0)
    {
        print_list(head);
    }
    else if (strcmp(input_string, "fg") == 0)
    {
        if (head == NULL)
        {
            printf("fg: no jobs\n");
            return;
        }
        kill(head->Pid, SIGCONT);
        waitpid(head->Pid, &status, WUNTRACED);
        delete_from_first(&head);
    }
    else if (strcmp(input_string, "bg") == 0)
    {
        if (head == NULL)
        {
            printf("fg: no jobs\n");
            return;
        }
        signal(SIGCHLD, signal_handler);
        kill(head->Pid, SIGCONT);
        delete_from_first(&head);
    }
}

void execute_external_command(char *input_string)
{
    char *argv[10]; // array of pointers
    int i = 0, j = 0, k = 0;

    char temp[100]; // temp buffer for one argument

    while (input_string[k] != '\0')
    {
        if (input_string[k] == ' ')
        {
            if (j > 0)
            {
                temp[j] = '\0';
                argv[i] = malloc(strlen(temp) + 1);
                strcpy(argv[i], temp);
                i++;
                j = 0;
            }
        }
        else
        {
            temp[j++] = input_string[k];
        }
        k++;
    }

    // last argument
    if (j > 0)
    {
        temp[j] = '\0';
        argv[i] = malloc(strlen(temp) + 1);
        strcpy(argv[i], temp);
        i++;
    }
    argv[i] = NULL;

    int flag = 0;
    for (i = 0; input_string[i]; i++)
    {
        if (input_string[i] == '|')
        {
            flag = 1;
        }
    }
    if (!flag)
    {
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    else
    {
        int argc = 0;
        while (argv[argc] != NULL)
            argc++;

        int cmd_index[20];
        int cmd_count = 0;

        cmd_index[cmd_count++] = 0;

        for (i = 0; i < argc; i++)
        {
            if (strcmp(argv[i], "|") == 0)
            {
                argv[i] = NULL;
                cmd_index[cmd_count++] = i + 1;
            }
        }

        int pipefd[2];
        int in_fd = 0;

        for (i = 0; i < cmd_count; i++)
        {
            if (i < cmd_count - 1)
                pipe(pipefd);

            int pid1 = fork();

            if (pid1 == 0)
            {
                dup2(in_fd, 0);

                if (i < cmd_count - 1)
                    dup2(pipefd[1], 1);

                if (i < cmd_count - 1)
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                }

                execvp(argv[cmd_index[i]], &argv[cmd_index[i]]);
                perror("execvp");
                exit(1);
            }
            else
            {
                waitpid(pid1, NULL, 0);

                if (i < cmd_count - 1)
                {
                    close(pipefd[1]);
                    in_fd = pipefd[0];
                }
            }
        }
        exit(0);
    }
}

void print_list(S *head)
{
    if (head == NULL)
    {
        printf("INFO: No pending processes\n");
    }
    else
    {
        while (head)
        {
            printf("%d\t", head->Pid);
            printf("%s\n", head->command);
            head = head->link;
        }
        }
}

int delete_from_first(S **head)
{
    if (*head == NULL)
    {
        return 1;
    }
    S *temp = *head;
    *head = temp->link;

    free(temp);

    return 0;
}
