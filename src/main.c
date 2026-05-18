/*
 * ccowsay: a small C remake of cowsay.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccowsay.h"

static void displayhelp(void);
static void displaycowlist(void);
static int is_program(const char *path, const char *name);

int main(int argc, char *argv[])
{
    char eyes = 'o';
    char upperbubble = '\\';
    char lowerbubble = '\\';
    const char *cowname = "default";
    unsigned int argscharcount = 0;
    unsigned short thought = 0;

    if (is_program(argv[0], "cowthink")) {
        thought = 1;
        upperbubble = 'O';
        lowerbubble = 'o';
    }

    if (argc == 1) {
        displayhelp();
        exit(EXIT_FAILURE);
    }

    for (int counter = 1; counter < argc; counter++) {
        if (!strcmp(argv[counter], "-e") || !strcmp(argv[counter], "--eyes")) {
            int nextarg = counter + 1;
            if (nextarg < argc && strlen(argv[nextarg]) == 1) {
                eyes = argv[nextarg][0];
                counter++;
            } else {
                displayhelp();
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(argv[counter], "-t") || !strcmp(argv[counter], "--thought")) {
            thought = 1;
            upperbubble = 'O';
            lowerbubble = 'o';
        } else if (!strcmp(argv[counter], "-c") || !strcmp(argv[counter], "--cow")) {
            int nextarg = counter + 1;
            if (nextarg < argc && ccowsay_find_cow(argv[nextarg]) != NULL) {
                cowname = argv[nextarg];
                counter++;
            } else {
                displaycowlist();
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(argv[counter], "-h") || !strcmp(argv[counter], "--help")) {
            displayhelp();
            exit(EXIT_SUCCESS);
        } else if (!strcmp(argv[counter], "-l") || !strcmp(argv[counter], "--list")) {
            displaycowlist();
            exit(EXIT_SUCCESS);
        }
    }

    for (int counter = 1; counter < argc; counter++) {
        if (!strcmp(argv[counter], "-c") || !strcmp(argv[counter], "--cow") ||
            !strcmp(argv[counter], "-e") || !strcmp(argv[counter], "--eyes")) {
            counter++;
        } else if (!strcmp(argv[counter], "-t") || !strcmp(argv[counter], "--thought")) {
            continue;
        } else {
            argscharcount = argscharcount + 1 + strlen(argv[counter]);
        }
    }

    if (argscharcount == 0) {
        displayhelp();
        exit(EXIT_FAILURE);
    }
    argscharcount++;

    printf(" ");
    for (unsigned int counter = 1; counter <= argscharcount; counter++) {
        printf("_");
    }

    printf(thought ? "\n( " : "\n< ");

    for (int counter = 1; counter < argc; counter++) {
        if (!strcmp(argv[counter], "-c") || !strcmp(argv[counter], "--cow") ||
            !strcmp(argv[counter], "-e") || !strcmp(argv[counter], "--eyes")) {
            counter++;
        } else if (!strcmp(argv[counter], "-t") || !strcmp(argv[counter], "--thought")) {
            continue;
        } else {
            printf("%s ", argv[counter]);
        }
    }

    printf(thought ? ")\n " : ">\n ");
    for (unsigned int counter = 1; counter <= argscharcount; counter++) {
        printf("-");
    }
    printf("\n");

    ccowsay_print_cow(stdout, cowname, eyes, upperbubble, lowerbubble);
    return 0;
}

static void displaycowlist(void)
{
    size_t count;
    const ccowsay_cow *cows = ccowsay_cows(&count);

    for (size_t i = 0; i < count; i++) {
        printf("%s\t%s\n", cows[i].name, cows[i].description);
    }
}

static void displayhelp(void)
{
    printf("This program displays all of its arguments in a speech bubble.\n"
           "With your choice of animal/person/thing underneath it. Or in\n"
           "other words this program is a re-make of the famous Perl\n"
           "program ``cowsay\"\n"
           "\n"
           "Usage:\n"
           "    The flags should be the first set of arguments.\n"
           "    ccowsay <flag(s)> Sentence to say\n"
           "\n"
           "Flags:\n"
           "    -h or --help    - Displays this help text\n"
           "    -l or --list    - List all cowfiles\n"
           "    -c or --cow     - Specify cowfile\n"
           "    -e or --eyes    - Specify character used for the eyes. Must be ONE character!\n"
           "    -t or --thought - Thought bubble\n");
}

static int is_program(const char *path, const char *name)
{
    const char *base = strrchr(path, '/');

    if (base == NULL) {
        base = path;
    } else {
        base++;
    }

    return strcmp(base, name) == 0;
}
