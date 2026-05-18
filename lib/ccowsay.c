#include "ccowsay.h"

#include <string.h>

static const ccowsay_cow cows[] = {
    {
        "default",
        "Classic ccowsay cow",
        "        %c   ^__^\n"
        "         %c  (%c%c)\\_______\n"
        "            (__)\\       )\\/\\\n"
        "                ||----w |\n"
        "                ||     ||\n"
    },
    {
        "small",
        "Small cow",
        "      %c  ^__^\n"
        "       %c (%c%c)\\_______\n"
        "         (__)\\       )\\/\\\n"
        "             ||----w |\n"
    },
    {
        "sheep",
        "Woolly sheep",
        "     %c\n"
        "      %c  __  _\n"
        "        .-.'  `; `-._  __  _\n"
        "       (_,         .-:'  `; `-._\n"
        "     ,'%c%c)        (_,           )\n"
        "    (__,-'      ,'%c%c)          /\n"
        "       (       (__,-'          /\n"
        "        `-'._.--._(           /\n"
        "           |||  |||`-'._.--._.'\n"
    },
    {
        "tux",
        "Small freehand Tux-like penguin",
        "        %c\n"
        "         %c\n"
        "        .--.\n"
        "       |%c_%c |\n"
        "       |:_/ |\n"
        "      //   \\ \\\n"
        "     (|     | )\n"
        "    /'\\_   _/`\\\n"
        "    \\___)=(___/\n"
    },
    {
        "dragon",
        "Tiny dragon",
        "        %c     /\\\n"
        "         %c   /%c%c \\\n"
        "            /   __\\\n"
        "           /___/  \\"
    },
};

const ccowsay_cow *ccowsay_cows(size_t *count)
{
    if (count != NULL) {
        *count = sizeof(cows) / sizeof(cows[0]);
    }
    return cows;
}

const ccowsay_cow *ccowsay_find_cow(const char *name)
{
    size_t count;
    const ccowsay_cow *list = ccowsay_cows(&count);

    if (name == NULL || name[0] == '\0') {
        name = "default";
    }

    for (size_t i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];
        }
    }

    return NULL;
}

void ccowsay_print_cow(FILE *stream, const char *name, char eyes, char upperbubble, char lowerbubble)
{
    const ccowsay_cow *cow = ccowsay_find_cow(name);

    if (cow == NULL) {
        cow = ccowsay_find_cow("default");
    }

    if (strcmp(cow->name, "sheep") == 0) {
        fprintf(stream, cow->template, upperbubble, lowerbubble, eyes, eyes, eyes, eyes);
    } else {
        fprintf(stream, cow->template, upperbubble, lowerbubble, eyes, eyes);
    }
}
