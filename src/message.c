#include "ccowsay.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int append_arg(char** text, const char* arg)
{
    size_t old_len = *text ? strlen(*text) : 0;
    size_t arg_len = strlen(arg);
    size_t spacer = old_len ? 1 : 0;
    char* next = realloc(*text, old_len + spacer + arg_len + 1);

    if (!next)
        return EXIT_FAILURE;

    if (spacer)
        next[old_len++] = ' ';
    memcpy(next + old_len, arg, arg_len + 1);
    *text = next;
    return EXIT_SUCCESS;
}

static int push_line(Lines* lines, const char* start, size_t len)
{
    char** items;
    char* line = malloc(len + 1);

    if (!line)
        return EXIT_FAILURE;

    memcpy(line, start, len);
    line[len] = '\0';

    items = realloc(lines->items, sizeof(*items) * (lines->count + 1));
    if (!items) {
        free(line);
        return EXIT_FAILURE;
    }

    lines->items = items;
    lines->items[lines->count++] = line;
    return EXIT_SUCCESS;
}

int read_stdin_text(char** out)
{
    size_t len = 0;
    size_t cap = 0;
    int ch;
    char* text = NULL;

    while ((ch = getchar()) != EOF) {
        if (len + 1 >= cap) {
            size_t next_cap = cap ? cap * 2 : 256;
            char* next = realloc(text, next_cap);

            if (!next) {
                free(text);
                return EXIT_FAILURE;
            }
            text = next;
            cap = next_cap;
        }
        text[len++] = (char)ch;
    }

    if (!text) {
        *out = NULL;
        return EXIT_SUCCESS;
    }

    text[len] = '\0';
    *out = text;
    return EXIT_SUCCESS;
}

static int split_lines(const char* text, Lines* lines)
{
    const char* line = text;
    const char* p;

    for (p = text; *p; p++) {
        if (*p == '\n') {
            size_t len = (size_t)(p - line);

            if (len && line[len - 1] == '\r')
                len--;
            if (push_line(lines, line, len) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            line = p + 1;
        }
    }

    if (line != p) {
        size_t len = (size_t)(p - line);

        if (len && line[len - 1] == '\r')
            len--;
        return push_line(lines, line, len);
    }

    if (!lines->count)
        return push_line(lines, "", 0);

    return EXIT_SUCCESS;
}

static int push_wrapped_line(Lines* lines, char* line, size_t* line_len)
{
    int result = push_line(lines, line, *line_len);

    *line_len = 0;
    line[0] = '\0';
    return result;
}

static int wrap_words(const char* text, int width, Lines* lines)
{
    size_t cap = (size_t)width + 1;
    char* line = calloc(cap, 1);
    size_t line_len = 0;
    const char* p = text;

    if (!line)
        return EXIT_FAILURE;

    while (*p) {
        const char* word;
        size_t word_len;

        while (*p && isspace((unsigned char)*p))
            p++;
        if (!*p)
            break;

        word = p;
        while (*p && !isspace((unsigned char)*p))
            p++;
        word_len = (size_t)(p - word);

        if (word_len > (size_t)width) {
            if (line_len && push_wrapped_line(lines, line, &line_len) != EXIT_SUCCESS) {
                free(line);
                return EXIT_FAILURE;
            }
            if (push_line(lines, word, word_len) != EXIT_SUCCESS) {
                free(line);
                return EXIT_FAILURE;
            }
            continue;
        }

        if (line_len && line_len + 1 + word_len > (size_t)width) {
            if (push_wrapped_line(lines, line, &line_len) != EXIT_SUCCESS) {
                free(line);
                return EXIT_FAILURE;
            }
        }

        if (line_len)
            line[line_len++] = ' ';
        memcpy(line + line_len, word, word_len);
        line_len += word_len;
        line[line_len] = '\0';
    }

    if (line_len) {
        if (push_line(lines, line, line_len) != EXIT_SUCCESS) {
            free(line);
            return EXIT_FAILURE;
        }
    } else if (!lines->count && push_line(lines, "", 0) != EXIT_SUCCESS) {
        free(line);
        return EXIT_FAILURE;
    }

    free(line);
    return EXIT_SUCCESS;
}

int split_or_wrap_text(const char* text, int width, int no_wrap, Lines* lines)
{
    if (width < 1)
        width = 40;

    if (no_wrap)
        return split_lines(text, lines);

    return wrap_words(text, width, lines);
}

void free_lines(Lines* lines)
{
    for (size_t i = 0; i < lines->count; i++) {
        free(lines->items[i]);
    }
    free(lines->items);

    lines->items = NULL;
    lines->count = 0;
}
