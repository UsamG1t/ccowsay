#ifndef CCOWSAY_H
#define CCOWSAY_H

#include <stddef.h>
#include <stdio.h>

#define CCOWSAY_VERSION "0.1.0"

#ifndef DEFAULT_COWDIR
#define DEFAULT_COWDIR "/usr/share/ccowsay/cows"
#endif

typedef struct {
    char eyes[3];
    char tongue[3];
    char cow[256];
    int think;
    int no_wrap;
    int wrap_width;
} CcowsayOptions;

typedef struct {
    char** names;
    size_t count;
} CowList;

typedef struct {
    char** items;
    size_t count;
} Lines;

int append_arg(char** text, const char* arg);
int read_stdin_text(char** out);
int split_or_wrap_text(const char* text, int width, int no_wrap, Lines* lines);
void free_lines(Lines* lines);

void print_balloon(FILE* out, const Lines* lines, int think);
void apply_face_mode(CcowsayOptions* opts, char mode);

int render_cow(FILE* out, const char* name, const CcowsayOptions* opts);
int list_cows(FILE* out);

#endif
