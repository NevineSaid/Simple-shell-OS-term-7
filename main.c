#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h> //installed library

char **get_instr(char *in);
int cd(char *dir);
void handler(int s);

int main()
{
    signal(SIGCHLD,handler);

    pid_t child_pid;

    int stat;
    char **instr;
    char *in;




    while (1)
    {
        in = readline("myShell> ");
        instr = get_instr(in);

        if (strcmp(instr[0], "cd") == 0)
        {
            if (cd(instr[1]) < 0) //if it has no args
            {
                //do nothing
            }

            continue;
        }
        if (!instr[0])        //null instr
        {
            free(in);
            free(instr);
            continue;
        }
        if(strcmp(instr[0],"exit")==0)
        {
            exit(0);
        }


        child_pid = fork();

        if (child_pid == 0)   //if it has no child then this is the child
        {

            execvp(instr[0], instr);
            printf("unsupported or wrong spelling!\n");
        }
        else
        {
            //if(!strchr(strlen(instr)-1,"&")){
            waitpid(child_pid, &stat, WUNTRACED);
            //}

        }

        free(in);
        free(instr);
    }

    return 0;
}

//a function used to parse the entered string and returns a ptr to an array of characters
char **get_instr(char *in)
{
    char **instr = malloc( 10* sizeof(char *));
    char *separate = " ";
    char *words;
    int index = 0;

    words = strtok(in, separate); //tokeniser

    while (words != NULL)
    {
        instr[index] = words;
        index++;
        words = strtok(NULL, separate);
    }

    instr[index] = NULL;
    return instr;
}
int cd(char *dir)
{
    return chdir(dir); //returns current directory
}
void handler(int s)
{
    FILE *f;
    f = fopen("log_file.txt","a");
    fputs("child process was terminated!\n",f);
    fclose(f);
    pid_t pid;
    pid = wait(NULL);
}
