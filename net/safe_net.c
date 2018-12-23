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

char const * make_usage(char const * const name)
{
    char const * const pattern = "%s sam set pwdmustchangenow USERNAME yes\n";
    int const size = strlen(pattern) + strlen(name) - 1;
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
    *destination = malloc(strlen(source));
    return strcpy(*destination,source);
}

int main (int const argc, char const * const argv[])
{
    char const * const usage = make_usage(argv[0]);
    char const * const file = "/usr/bin/net";

    pid_t pid;
    pid_t ret;
    int status;
    char * command[7];

    int index = 0;
    int reti;
    regex_t regex;
    char const * username;

    if(argc != 6)
    {
        fprintf(stderr, usage);
        return 1;
    }

    if(!streq(argv[1], "sam") || !streq(argv[2],"set") ||
            !streq(argv[3],"pwdmustchangenow") || !streq(argv[5],"yes"))
    {
        fprintf(stderr, usage);
        return 1;
    }

    mstrcpy(&command[index++],file);
    mstrcpy(&command[index++],argv[1]);
    mstrcpy(&command[index++],argv[2]);
    mstrcpy(&command[index++],argv[3]);

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

    mstrcpy(&command[index++],username);
    mstrcpy(&command[index++],argv[5]);
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

    if (execve(file, command, NULL) == -1)
        return 127;

}
