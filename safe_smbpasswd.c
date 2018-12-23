#include <stdio.h>
#include <regex.h> 
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

/* Runs /usr/bin/smbpasswd -s (-x|-a)? USERNAME
 * */

char const * make_usage(char const * const name)
{
    char const * const pattern = "%s [-a|-x] USERNAME\n";
    int const size = strlen(pattern) + strlen(name) - 1;
    char * buffer = malloc(size);
    sprintf(buffer,pattern,name);
    return buffer;
}

int streq(char const * const left, char const * const right)
{
    return strcmp(left,right) == 0;
}

int main (int const argc, char const * const argv[])
{
    char const * const usage = make_usage(argv[0]);
    char const * const file = "/usr/bin/smbpasswd";

    pid_t pid;
    pid_t ret;
    /* int timeout = 2000; */
    int status;
    char * command[5];

    int index = 0;
    int reti;
    regex_t regex;
    char const * username;

    /* TODO disposable */
    /* int i; */

    if(argc < 2 || argc > 3)
    {
        fprintf(stderr, usage);
        return 1;
    }

    if(argc == 3 && !streq(argv[1], "-a") && !streq(argv[1],"-x"))
    {
        fprintf(stderr, usage);
        return 1;
    }

    strcpy(command[index++],file);
    strcpy(command[index++],"-s");

    if(argc == 3)
    {
        strcpy(command[index++],argv[1]);
        username = argv[2];
    }
    else
        username = argv[1];


    /* Username regex.
     * */
    reti = regcomp(&regex, "^[a-zA-Z][0-9a-zA-Z._\\-]+$", REG_EXTENDED);

    if(reti)
    {
        fprintf(stderr, "Could not compile regex.\n");
        return 1;
    }

    reti = regexec(&regex, username, 0, NULL, 0);

    if(reti)
    {
        fprintf(stderr, "Illegal username.\n");
        return 1;
    }

    if(streq(username,"root"))
    {
        fprintf(stderr, "root not allowed.\n");
        return 1;
    }

    strcpy(command[index++],username);
    command[index++] = NULL;

    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Could not fork process.\n");
        return 1;
    }

    if(pid > 0)
    {
        while ((ret = waitpid(pid, &status, 0)) == -1)
        {
            if (errno != EINTR)
                return errno;
        }

        if ((ret != -1) && (!WIFEXITED(status) || !WEXITSTATUS(status)) )
            return WEXITSTATUS(status);

        return 0;
    }

    /* printf("%s",command[0]); */
    /* for(i=1; i<5;i++) */
    /* { */
    /*     if(command[i] == NULL) */
    /*         break; */
    /*     printf(" %s",command[i]); */
    /* } */
    /* printf("\n"); */

    if (execve(file, command, NULL) == -1)
        return 127;

    /* while (0 == (waitret = waitpid(pid, &status, WNOHANG))) */
    /* { */
    /*     timeout = timeout - 100; */

    /*     if(timeout < 0) */
    /*     { */
    /*         fprintf(stderr, "Timeout.\n"); */
    /*         return -1; */
    /*     } */

    /*     sleep(100); */
    /* } */

    /* if ((ret != -1) && (!WIFEXITED(status) || !WEXITSTATUS(status)) ) */
    /*     return WEXITSTATUS(status); */

    /* return 0; */
}
