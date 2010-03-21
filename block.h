#ifndef __block_h
#define __block_h

#define VERSION "0.1"
#define DEFAULTTABLE "bruteforcers"

/* Command line options */
struct optlist {
    int      kflag;     /* --kill-states */
    int      nflag;     /* --no-add */
    char    *table;     /* --table */
    char    *progname;  /* name of executable */
};

/* Linked list containing IPs to ban. */
struct iplist {
    char *ip;   /* Ip address */
    struct iplist *next;
};

#endif /* !__block_h */
