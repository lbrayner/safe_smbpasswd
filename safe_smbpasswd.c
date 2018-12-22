#include <stdio.h>
#include <regex.h> 
#include <string.h>

/* Runs /usr/bin/smbpasswd -s (-x|-a)? USERNAME
 * */

char * usage()
{
    return "./safe_smbpasswd [-a|-x] USERNAME\n";
}

int streq(char * left, char * right)
{
    return strcmp(left,right) == 0;
}

int main (int argc, char *argv[])
{
    const char * const prefix = "/usr/bin/smbpasswd -s";
    int reti;
    regex_t regex;
    char optional[4] = "";
    const char * username;
    char command[300] = "";

    if(argc < 2 || argc > 3)
    {
        fprintf(stderr, usage());
        return 1;
    }

    if(argc == 3 && !streq(argv[1], "-a") && !streq(argv[1],"-x"))
    {
        fprintf(stderr, usage());
        return 1;
    }

    if(argc == 3)
    {
        sprintf(optional," %s",argv[1]);
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

    sprintf(command,"%s%s %s\n",prefix,optional,username);

    printf(command);

    return 0;
}
