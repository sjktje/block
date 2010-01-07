#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#define DEFAULTTABLE "badips"

void pfctldel(char *, char *);
void usage(void);

int 
main(int argc, char *argv[])
{
    int   ch;           /* getopt */
    char *ips = NULL;   /* Ip addresses in argv */
    char *table = NULL; /* pf table */

    while ((ch = getopt(argc, argv, "kt:")) != -1) {
        switch(ch) {
        case 't':
            table = optarg;
            break;
        case 'h':
        case '?':
            usage();
            break;
        }
    }

    argc -= optind;
    argv += optind;

    if (argc < 1) {
        usage();
        exit(1);
    }

    ips = getips(argc, argv);

    if (table == NULL) 
        asprintf(&table, "%s", DEFAULTTABLE);

    pfctldel(ips, table);

    return 0;
}

void
pfctldel(char *ips, char *table)
{
    int cmdlength = 0;
    char *cmd = NULL;

    asprintf(&cmd, "%spfctl -t %s -T del ",
            isroot() ? "" : "sudo ", table);
    
    cmdlength = strlen(cmd) + strlen(ips) + 1;
    cmd = realloc(cmd, cmdlength);

    strncat(cmd, ips, cmdlength);

    system(cmd);

    free(cmd);
    cmd = NULL;
}

void
usage(void)
{
    fprintf(stderr, "usage: unblock [-h] [-t table] ip ip ... ip\n");
    exit(1);
}
