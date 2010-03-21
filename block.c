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

#include <ctype.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "block.h"

static char             *getip(char **, char *);
static char             *vg_asprintf(const char *, ...);
static char             *vg_strdup(char *);
static int               exists_ip(struct iplist *, char *);
static int               is_empty(char *);
static int               systemf(const char *, ...);
static struct optlist   *optlistinit(void);
static struct optlist   *parseargs(int *, char ***);
static void              add_ip(struct iplist **, char *);
static void              ban_ips(struct iplist *, struct optlist *); 
static void              perrorf(const char *, ...);
static void              usage(char *);


int
main(int argc, char *argv[]) 
{
    struct optlist *cmdargs;
    struct iplist  *head = NULL;
    char buf[512];
    char *p = NULL;
    char *q = NULL;
    char *ip = NULL;

    cmdargs = parseargs(&argc, &argv);

    while ((p = fgets(buf, sizeof(buf), stdin)) != NULL) {

        if ((q = strchr(p, '\n')) == NULL) {
            fprintf(stderr, "Ignoring line that did not fit in buffer.\n");
            continue;
        }

        *q = '\0';
        q = p;

        while ((q = getip(&ip, q)) != NULL) {
            if (is_empty(ip))
                continue;
            if (exists_ip(head, ip))
                continue;
            add_ip(&head, ip);
        }

    }

    ban_ips(head, cmdargs);

    return 0;
}

/* 
 * Makes dst point to malloced string containing first IP address found in P.
 * Returns NULL if no IP address was found.
 */
static char *
getip(char **dst, char *p)
{
    int ip[4];
    int off;

    if (is_empty(p)) 
        return NULL;
    
    while (!isdigit(*p) && (*p != '\0'))
        p++;

    if (4 <= sscanf(p, "%d.%d.%d.%d%n", &ip[0], &ip[1], &ip[2], &ip[3], &off)) {
        *dst = vg_asprintf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        p += off;
        return p;
    } else {
        *dst = NULL;
        return p+1;
    }
}

/*
 * Prints help text and exits.
 */
static void
usage(char *p)
{
    fprintf(stderr, "Usage: %s [OPTION..]\n\n", p);

    fprintf(stderr,
            "   -h, --help                  this help text\n"
            "   -k, --kill-states           kill states\n"
            "   -n, --no-add                only kill states\n"
            "   -t, --table                 table to add IPs to\n"
            "   -v, --version               display program version\n\n"
            "See http://ankeborg.nu/wiki/block for more information.\n");
    exit(1);
}

/*
 * Parses command line arguments and returns a malloced struct optlist 
 * containing flags and arguments issued by the user.
 */
struct optlist *
parseargs(int *argc, char ***argv)
{
    int             ch;
    struct optlist *cmdargs;
    static struct option options[] = {
        { "table",              required_argument,  NULL,   't' },
        { "kill-states",        no_argument,        NULL,   'k' },
        { "no-add",             no_argument,        NULL,   'n' },
        { "version",            no_argument,        NULL,   'v' },
        { "help",               no_argument,        NULL,   'h' },
    };

    cmdargs = optlistinit();
    cmdargs->progname = *argv[0];

    while ((ch = getopt_long(*argc, *argv, "hknt:v", options, NULL)) != -1) {
        switch (ch) {
        case 'h':
            usage(*argv[0]);
            break; /* NOT REACHED */
        case 'k':
            cmdargs->kflag = 1;
            break;
        case 'n':
            cmdargs->nflag = 1;
            break;
        case 't':
            cmdargs->table = vg_strdup(optarg);
            break;
        case 'v':
            printf("%s v%s\n", cmdargs->progname, VERSION);
            exit(0);
            break; /* NOT REACHED */
        default:
            usage(*argv[0]);
            break; /* NOT REACHED */
        }
    }

    *argc -= optind;
    *argv += optind;

    if (is_empty(cmdargs->table))
        cmdargs->table = strdup(DEFAULTTABLE);

    return cmdargs;
}

/*
 * Initializes optlist struct used by parseargs()
 */
static struct optlist *
optlistinit(void)
{
    struct optlist *cmdargs;
    if ((cmdargs = malloc(sizeof(struct optlist))) == NULL) {
        perror("optlistinit");
        exit(1);
    }

    cmdargs->kflag = 0;
    cmdargs->nflag = 0;
    cmdargs->table = NULL;
    return cmdargs;
}

/*
 * strdup() that exits upon error.
 */
static char *
vg_strdup(char *src)
{
    char *dst;
    if ((dst = strdup(src)) == NULL) {
        perror("vg_strdup");
        exit(1);
    }
    return dst;
}

/* 
 * asprintf() that exits upon error.
 */
static char *
vg_asprintf(const char *fmt, ...)
{
    va_list va;
    char *dst = NULL;
    va_start(va, fmt);
    vasprintf(&dst, fmt, va);
    if (dst == NULL) {
        perror("vg_asprintf");
        exit(1);
    }
    va_end(va);
    return dst;
}

/*
 * printf-ish perror()
 */
static void
perrorf(const char *fmt, ...)
{
    char buf[128];
    va_list va;
    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    perror(buf);
}

/* 
 * printf-ish system()
 */
static int
systemf(const char *fmt, ...) {
    char *string = NULL;
    va_list va;
    int ret;

    va_start(va, fmt);
    if (vasprintf(&string, fmt, va) == -1) {
        perror("vasprintf");
        exit(1);
    }
    va_end(va);

    ret = system(string);
    free(string);
    return ret;
}

/* 
 * Checks if string is empty (NULL or \0) and returns true if that is the case.
 */
static int
is_empty(char *s)
{
    if ((s == NULL) || (*s == '\0'))
        return 1;
    else
        return 0;
}


/* 
 * Adds ip to linked list. 
 */
static void add_ip(struct iplist **head, char *ip) {
    struct iplist *new;
    if ((new = malloc(sizeof(struct iplist))) == NULL) {
        perror("add_ip");
        exit(1);
    }
    new->ip = strdup(ip);
    new->next = *head;
    *head = new;
}

/* 
 * Checks if ip is already on the list. 
 */
static int exists_ip(struct iplist *head, char *ip) {
    int len;

    if (head == NULL)
        return 0;

    len = strlen(ip);

    while (head != NULL) {
        if ((strncmp(head->ip, ip, len)) == 0)
            return 1;
        head = head->next;
    }
    return 0;
}

/* 
 * Adds ips in linked list to PF table t or kills their states or both, 
 * depending on command line arguments.
 */
static void 
ban_ips(struct iplist *head, struct optlist *c) 
{
    FILE *PFCTL = NULL;
    char *cmd = NULL;

    if (!c->nflag) {
        cmd = vg_asprintf("/sbin/pfctl -t %s -T add -f -", c->table);

        if ((PFCTL = popen(cmd, "w")) == NULL) {
            perrorf("Could not popen %s", cmd);
            exit(1);
        }

        free(cmd);
    }

    while (head != NULL) {
        if (!c->nflag)
            fprintf(PFCTL, "%s\n", head->ip);
        if (c->kflag)
            systemf("pfctl -k %s", head->ip);
        head = head->next;
    }

    pclose(PFCTL);
}

