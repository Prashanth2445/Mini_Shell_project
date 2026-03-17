/*
 * Name:Prashanth reddy
 * A.no:25021_303
 * Date:18/02/2026
 *Description:
    MiniShell is a console-based project written in C that simulates the basic working of a Linux shell by reading user commands,
    identifying built-in and external commands, and executing them using system calls like fork(), execvp(), and waitpid().
    It supports common shell features such as custom prompt (PS1=), command execution with arguments,
    pipe handling (|), signal handling for Ctrl+C and Ctrl+Z, job control (jobs, fg, bg), and printing exit status using echo $?.

*/


#include <stdio.h>
#include "headfile.h"

char prompt[25] = "minishell$: ";
char input_string[25];
char *external_commands[152];

/*INTERNAL commands 2d array*/
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg","bg", NULL};

int main()
{

    system("clear");       // Clear the terminal screen
    scan_input(prompt, input_string);  // user input string reading 
}