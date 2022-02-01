/* Name: Gabriel Gherasim
** Email: gherasimg15@gmail.com
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "functions.h"

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\n%s# ^C", timeInfo());
        printf("\n%s# ", timeInfo());
        fflush(stdout);
    }
}

void removeTrailingNL(char st[])
{
    size_t newLine = strlen(st) - 1;
    st[newLine] = '\0';
}

void runcmd(char *s, char *args[])
{
    pid_t pid;
    int child_status;
    char *bin = "/bin/";
    int len = strlen(bin) + strlen(s);
    char path[len];
    strcpy(path, bin);
    strcat(path, s);

    pid = fork();
    if (pid == 0)
    {
        execvp(path, args);
        printf("\n >> Invalid command << \n");
        exit(0);
    }
    else
    {
        wait(&child_status);
    }
}

void print(char **strs)
{
    printf("\n");
    for (int x = 0; strs[x] != NULL; x++)
    {
        printf("%s ", strs[x]);
        if (x != 0 && x % 20 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}
void lowercase(char *s)
{
    int x = 0;
    while (s[x] != '\0')
    {
        s[x] = tolower(s[x]);
        x++;
    }
}

void cdcmd(char *str)
{
    char *home = getenv("HOME");
    int er = 0;
    char cwd[100];

    if (strcmp("home", str) != 0)
    {
        er = chdir(str);
    }
    else
    {
        er = chdir(home);
    }

    if (er != 0)
    {
        perror("cd");
    }
}

void whoami()
{
    char *user = getenv("USER");
    printf("User: %s\n", user);
}

char *timeInfo()
{
    char *buff;
    time_t current_time;
    size_t size = 70;
    buff = malloc(size * sizeof(char));
    time(&current_time);

    strftime(buff, size, "[%d/%b %I:%M%p]", localtime(&current_time));

    return buff;
}

void help()
{
    printf("\nHELP PAGE:\n"
           "\nAvalable Commands:"
           "\t[ls] Lists files in directory"
           "\n\t[mkdir] Creates a folder/directory with the given name"
           "\n\t[pwd] Prints out the path of the current directory"
           "\n\t[echo] Prints out the given string given as argument"
           "\n\t[sleep] Pauses for the amount of seconds given"
           "\n\t[rmdir] Removes a directory from the current dicrectory"
           "\n\t[rm] Removes a file from the current dicrectory"
           "\n\t[cat] Prints out the file/s given to the terminal"
           "\n\t[whoami] Prints out the name of the current user"
           "\n\t[exit] Exits the program/shell"
           "\n\t[clear] Clear the shell screen"
           "\n\t[args] Prints out all of the arguments given"
           "\n\t[help] Prints out minimal information about the commands"
           "\n\t[man] Prints out info about given commands"
           "\n\t[cd] Changes the current working directory. Changes to home directory if no arguments given"
           "\n\n\t[\tAditional information\t]"
           "\n\t >>Using a commands followed by '>' will "
           "\n\t >>print the result in the file given after the '>' sign [eg. ls > stdout]"
           "\n\t >>For more info on the commands use the man command\n");
}

int redirect(char **tokens, int *terminalOut)
{
    int index = 0;
    char *file_name = NULL;
    int f_id;
    int error;
    *terminalOut = dup(1);
    for (int x = 0; tokens[x] != NULL; x++)
    {

        if (strcmp(">", tokens[x]) == 0)
        {
            index = x;
            break;
        }
    }

    // if the < is entered before any commands return 0
    if (index < 1)
    {
        return 0;
    }

    if ((file_name = tokens[index + 1]) != NULL)
    {
        f_id = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (f_id == -1)
        {
            error = errno;
            fprintf(stderr, "Redirect error: %s", strerror(error));
            return 0;
        }
        else
        {
            if (dup2(f_id, 1) == -1)
            {
                error = errno;
                fprintf(stderr, "Redirect error: %s", strerror(error));
            }
        }
    }
    return index;
}