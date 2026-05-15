#include "ccowsay.h"

#include <stdio.h>
#include <string.h>

static size_t max_line_len(const Lines* lines)
{
    size_t max = 0;
    size_t i;

    for (i = 0; i < lines->count; i++) {
        size_t len = strlen(lines->items[i]);

        if (len > max)
            max = len;
    }

    return max;
}

static void print_border(FILE* out, char* border, size_t width)
{
    size_t i;

    fputc(' ', out);
    for (i = 0; i < width + 2; i++)
        fputs(border, out);
    fputc('\n', out);
}

static void print_text_line(FILE* out, char left, const char* text, char right, size_t width)
{
    size_t len = strlen(text);

    fprintf(out, "%c %s%*s %c\n", left, text, (int)(width - len), "", right);
}

void print_balloon(FILE* out, const Lines* lines, int think)
{
    size_t width = max_line_len(lines);
    size_t i;
    
    print_border(out, "_", width);
    
    if (lines->count == 1) {
        print_text_line(out, think ? '(' : '<', lines->items[0], think ? ')' : '>', width);
    } else if (think) {
        for (i = 0; i < lines->count; i++)
            print_text_line(out, '(', lines->items[i], ')', width);
    } else {
        print_text_line(out, '/', lines->items[0], '\\', width);
        for (i = 1; i + 1 < lines->count; i++)
            print_text_line(out, '|', lines->items[i], '|', width);
        print_text_line(out, '\\', lines->items[lines->count - 1], '/', width);
    }

    print_border(out, "¯", width);
}
