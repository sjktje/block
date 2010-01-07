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

#include "common.h"

void  pfctladd(char *, char *);
void  pfctlkill(char *);
void  usage(void);

/*
 * block adds specified IP addresses to a pf table 
 */
int 
main(int argc, char *argv[]) 
{
    int   kflag = 0;      /* -k kills states */
    int   Kflag = 0;      /* -K only kills states */
    int   ch;             /* Getopt */
    char *ips = NULL;     /* Ip addresses in argv */
    char *table = NULL;   /* Table to add addresses to */  

    while ((ch = getopt(argc, argv, "hkKt:")) != -1) {
        switch(ch) {
        case 'k':
            kflag = 1;
            break;
        case 'K':
            Kflag = 1;
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

    if (argc < 1) {
        usage();
        exit(1);
    }

    ips = getips(argc, argv);

    if (table == NULL) 
        asprintf(&table, "%s", DEFAULTTABLE);


    if (!Kflag)
        pfctladd(ips, table);

    if (kflag || Kflag) 
        pfctlkill(ips);

    return 0;
}

/*
 * Kills states originating from IP addresses provided via commandline.
 */
void
pfctlkill(char *ips)
{
    char *ip = NULL;
    char *cmd = NULL;
    int   cmdlength = 0;

    while((ip = strsep(&ips, " ")) != NULL) {
        if (ip[0] == '\0')
            continue;

        asprintf(&cmd, "%spfctl -k ", isroot() ? "" : "sudo ");
        cmdlength = strlen(cmd) + strlen(ip) + 1;

        cmd = realloc(cmd, cmdlength);
        strncat(cmd, ip, cmdlength);

        system(cmd);
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
    fprintf(stderr, "usage: block [-hkK] [-t table] ip ip ... ip\n");
    exit(1);
}
