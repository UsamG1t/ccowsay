#include "ccowsay.h"

#include <string.h>

void apply_face_mode(CcowsayOptions* opts, char mode)
{
    switch (mode) {
    case 'b':
        strcpy(opts->eyes, "==");
        break;
    case 'd':
        strcpy(opts->eyes, "xx");
        strcpy(opts->tongue, "U ");
        break;
    case 'g':
        strcpy(opts->eyes, "$$");
        break;
    case 'p':
        strcpy(opts->eyes, "@@");
        break;
    case 's':
        strcpy(opts->eyes, "**");
        strcpy(opts->tongue, "U ");
        break;
    case 'w':
        strcpy(opts->eyes, "OO");
        break;
    case 'y':
        strcpy(opts->eyes, "..");
        break;
    default:
        break;
    }
}
