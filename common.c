#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "common.h"
/*
 * Creates string consisting of all entries in *argv[].
 */
char *
getips(int argc, char *argv[]) 
{
    char *ips = NULL;
    int   length = 0;
    int   i;

    ips = malloc(15);

    for (i = 0; i < argc; i++) {

        length += strlen(argv[i])+1;

        if (ips == NULL) 
            ips = malloc(length);
        else
            ips = realloc(ips, length);

        strncat(ips, argv[i], length);
        strncat(ips, " ", length);

    }

    return ips;
}

char *
getstdinips(void)
{
    char *ips = NULL;
    char *line = NULL;
    char *tmpline = NULL;
    size_t len;
    int iplen = 0;

    while ((line = fgetln(stdin, &len))) {
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';
        else {
            if ((tmpline = malloc(len + 1)) == NULL) 
                /* error! */
                ;
            memcpy(tmpline, line, len);
            tmpline[len] = '\0';
            line = tmpline;
        }

        iplen += strlen(line)+1;

        if (ips == NULL)
            ips = malloc(iplen);
        else 
            ips = realloc(ips, iplen);

        strncat(ips, line, iplen);
        strncat(ips, " ", iplen);
    }

#ifdef DEBUG
    printf("getstdinips() - IP addresses are: %s\n", ips);
#endif

    return ips;
}

/* 
 * Returns 1 if we're root, 0 if not.
 */
int
isroot(void)
{
    uid_t uid = getuid();
    return (uid == 0) ? 1 : 0;
}
