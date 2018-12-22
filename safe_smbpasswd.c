#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{

     if (argc != 2) {
       printf ("USAGE: ./Person [-f|-l]\n");
       return 1;
     }
     if (strcmp (argv[1], "-f") == 0) {
       printf("First Name");
     }
     else if (strcmp (argv[1], "-l") == 0)
       printf("Last Name");
     }
     else {
       printf ("Unknown argument\n");
       return 1;
     }
     return 0;
}
