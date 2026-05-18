#ifndef CCOWSAY_H
#define CCOWSAY_H

#include <stdio.h>
#include <stddef.h>

typedef struct ccowsay_cow {
    const char *name;
    const char *description;
    const char *template;
} ccowsay_cow;

const ccowsay_cow *ccowsay_cows(size_t *count);
const ccowsay_cow *ccowsay_find_cow(const char *name);
void ccowsay_print_cow(FILE *stream, const char *name, char eyes, char upperbubble, char lowerbubble);

#endif
