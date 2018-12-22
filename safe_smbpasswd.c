#include <stdio.h>
/* #include <regex.h> */ 
#include <string.h>

/* Runs /usr/bin/smbpasswd -s (-x|-a)? USERNAME
 * */

char * usage()
{
    return "./safe_smbpasswd USERNAME\n";
}

int streq(char * left, char * right)
{
    return strcmp(left,right) == 0;
}

int main (int argc, char *argv[])
{
    const char * const prefix = "/usr/bin/smbpasswd -s";
    char optional[4] = "";
    char command[300] = "";

    if(argc < 2 || argc > 3)
    {
        printf(usage());
        return 1;
    }

    if(argc == 3 && !streq(argv[1], "-a") && !streq(argv[1],"-x"))
    {
        printf(usage());
        return 1;
    }

    if(argc == 3)
        sprintf(optional," %s",argv[1]);

    sprintf(command,"%s%s\n",prefix,optional);

    printf(command);

    return 0;
}
