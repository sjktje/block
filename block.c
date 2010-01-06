/*
 * Copyright (c) 2010 Svante J. Kvarnstrom <sjk@ankeborg.nu>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define DEFAULTTABLE "badips"

char *getips(int, char **);
void  pfctladd(char *, char *);
void  pfctlkill(int, char **);
void  usage(void);
int   isroot(void);

/*
 * block adds specified IP addresses to a pf table 
 */
int 
main(int argc, char *argv[]) 
{
    int   kflag = 0;      /* -k kills states */
    int   ch;             /* Getopt */
    char *ips = NULL;     /* Ip addresses in argv */
    char *table = NULL;

    while ((ch = getopt(argc, argv, "hkt:")) != -1) {
        switch(ch) {
        case 'k':
            kflag = 1;
            break;
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

    ips = getips(argc, argv);

    if (table == NULL) 
        asprintf(&table, "%s", DEFAULTTABLE);

    pfctladd(ips, table);

    if (kflag) 
        pfctlkill(argc, argv);

    return 0;
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

/*
 * Kills states originating from IP addresses provided via commandline.
 */
void 
pfctlkill(int argc, char *argv[])
{
    int   i;
    int   cmdlength = 0;
    char *cmd = NULL;

    for (i = 0; i < argc; i++) {
        asprintf(&cmd, "%spfctl -k ", isroot() ? "" : "sudo ");
        cmdlength = strlen(cmd) + strlen(argv[i]) + 1;

        cmd = realloc(cmd, cmdlength);
        
        strncat(cmd, argv[i], cmdlength);

        printf("%s\n", cmd);

        free(cmd);
        cmd = NULL;
    }
}


/* 
 * Adds IP addresses to specified table. Uses sudo if user is not root.
 */
void 
pfctladd(char *ips, char *table) 
{
    int cmdlength = 0;
    char *cmd = NULL;

    asprintf(&cmd, "%spfctl -t %s -T add ", 
            isroot() ? "" : "sudo ", table);

    cmdlength = strlen(cmd) + strlen(ips);
    cmd = realloc(cmd, cmdlength);

    strncat(cmd, ips, cmdlength);

    printf("%s\n", cmd);

    free(cmd);
    cmd = NULL;
}


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

void
usage(void)
{
    fprintf(stderr, "usage: block [-hk] [-t table] ip ip ... ip\n");
    exit(1);
}
