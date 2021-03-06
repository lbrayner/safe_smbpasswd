#include <stdio.h>
#include <regex.h> 
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

/* Runs /usr/bin/net sam set pwdmustchangenow USERNAME yes
 * */

char * make_usage(char const * const name)
{
    char const * const pattern = "%s sam set pwdmustchangenow USERNAME yes\n";
    int const size = strlen(pattern) - 1 /* the placeholder %s */ + strlen(name)
        + 1 /* the null character */;
    char * buffer = malloc(size);
    sprintf(buffer,pattern,name);
    return buffer;
}

int streq(char const * const left, char const * const right)
{
    return strcmp(left,right) == 0;
}

char * mstrcpy(char ** destination, char const * const source)
{
    *destination = malloc(strlen(source) + 1 /* the null character */);
    return strcpy(*destination,source);
}

void command_free(char * array[], int const length)
{
    int i;
    for(i=0;i<length;i++)
    {
        if(array[i] == NULL)
            break;

        free(array[i]);
    }
}

int main (int const argc, char const * const argv[])
{
    char * const usage = make_usage(argv[0]);
    char const * const file = "/usr/bin/net";

    pid_t pid;
    pid_t ret;
    int status;
    char * command[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    int index = 0;
    int reti;
    regex_t regex;
    char const * username;

    if(argc != 6)
    {
        fprintf(stderr, usage);
        free(usage);
        return 1;
    }

    if(!streq(argv[1], "sam") || !streq(argv[2],"set") ||
            !streq(argv[3],"pwdmustchangenow") || !streq(argv[5],"yes"))
    {
        fprintf(stderr, usage);
        free(usage);
        return 1;
    }

    /* We don't need this anymore.
     * */
    free(usage);

    username = argv[4];

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

    mstrcpy(&command[index++],file);
    mstrcpy(&command[index++],argv[1]);
    mstrcpy(&command[index++],argv[2]);
    mstrcpy(&command[index++],argv[3]);

    mstrcpy(&command[index++],username);
    mstrcpy(&command[index++],argv[5]);
    command[index++] = NULL;

    if(execve(file, command, NULL) == -1)
    {
        command_free(command,7);
        return 127;
    }
}
