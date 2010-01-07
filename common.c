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

/* 
 * Returns 1 if we're root, 0 if not.
 */
int
isroot(void)
{
    uid_t uid = getuid();
    return (uid == 0) ? 1 : 0;
}
