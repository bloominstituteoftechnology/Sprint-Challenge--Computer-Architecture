// gcc -Wall -Wextra -o fileread lecture2.c
// ./fileread

// **** just to make sure that is's the fist line:
// cat print8.ls8

// hexdump -C lecture2.c

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    char line[1024];

    fp = fopen("print8.ls8", "r");

    while (fgets(line, sizeof line, fp) != NULL)
    {
        printf("%s", line);

        if (line[0] == '\n' || line[0] == '#')
        {
            printf("Ignoring this line.\n");
            continue;
        }

        unsigned char b;
        b = strtoul(line, NULL, 2);

        printf("%02X\n", b);
    }

    fclose(fp);

    return 0;
}