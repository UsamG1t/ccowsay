#include "ccowsay.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(FILE* out)
{
    fprintf(out,
        "ccowsay %s\n"
        "Usage: ccowsay [options] <message>\n"
        "\n"
        "With no message, read standard input\n"
        "\n"
        "Options:\n"
        "  -h, --help           Show this help\n"
        "  -l, --list           List available cows\n"
        "  -f, --file <cow>     Select cow template\n"
        "  -c, --cow <cow>      Alias for -f\n"
        "  -e, --eyes <xx>      Set two eye characters\n"
        "  -T, --tongue <xx>    Set two tongue characters\n"
        "  -t, --thought        Render a thought bubble\n"
        "  -n, --no-wrap        Preserve input lines\n"
        "  -W, --wrap <cols>    Wrap message at column count\n"
        "  -b -d -g -p -s -w -y Face modes\n"
        "\n"
        "Cow path: CCOWSAY_COWPATH, then COWPATH, then " DEFAULT_COWDIR "\n",
        CCOWSAY_VERSION);
}

static int need_value(int argc, char** argv, int index)
{
    if (index + 1 < argc)
        return EXIT_SUCCESS;

    fprintf(stderr, "ccowsay: option '%s' needs a value\n", argv[index]);
    return EXIT_FAILURE;
}

static void set_pair(char out[3], const char* value)
{
    out[0] = value[0] ? value[0] : ' ';
    out[1] = value[1] ? value[1] : ' ';
    out[2] = '\0';
}

static int set_cow(CcowsayOptions* opts, const char* value)
{
    if (strlen(value) >= sizeof(opts->cow)) {
        fprintf(stderr, "ccowsay: cow name is too long\n");
        return EXIT_FAILURE;
    }

    strcpy(opts->cow, value);
    return EXIT_SUCCESS;
}

static int set_width(CcowsayOptions* opts, const char* value)
{
    char* end = NULL;
    long width;

    errno = 0;
    width = strtol(value, &end, 10);
    if (errno || end == value || *end || width < 1 || width > 10000) {
        fprintf(stderr, "ccowsay: invalid wrap width '%s'\n", value);
        return EXIT_FAILURE;
    }

    opts->wrap_width = (int)width;
    return EXIT_SUCCESS;
}

static int parse_args(int argc, char** argv, CcowsayOptions* opts, char** message)
{
    int after_dashdash = 0;
    int i;

    for (i = 1; i < argc; i++) {
        const char* arg = argv[i];

        if (after_dashdash) {
            if (append_arg(message, arg) != EXIT_SUCCESS) {
                fprintf(stderr, "ccowsay: out of memory\n");
                return EXIT_FAILURE;
            }
            continue;
        }

        if (strcmp(arg, "--") == 0) {
            after_dashdash = 1;
            continue;
        }

        if (arg[0] != '-' || !arg[1]) {
            if (append_arg(message, arg) != EXIT_SUCCESS) {
                fprintf(stderr, "ccowsay: out of memory\n");
                return EXIT_FAILURE;
            }
            continue;
        }

        if (arg[1] == '-') {
            if (!strcmp(arg, "--help")) {
                usage(stdout);
                exit(EXIT_SUCCESS);
            } else if (!strcmp(arg, "--list")) {
                exit(list_cows(stdout));
            } else if (!strcmp(arg, "--file") || !strcmp(arg, "--cow")) {
                if (need_value(argc, argv, i) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
                if (set_cow(opts, argv[++i]) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
            } else if (!strcmp(arg, "--eyes")) {
                if (need_value(argc, argv, i) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
                set_pair(opts->eyes, argv[++i]);
            } else if (!strcmp(arg, "--tongue")) {
                if (need_value(argc, argv, i) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
                set_pair(opts->tongue, argv[++i]);
            } else if (!strcmp(arg, "--thought")) {
                opts->think = 1;
            } else if (!strcmp(arg, "--no-wrap")) {
                opts->no_wrap = 1;
            } else if (!strcmp(arg, "--wrap")) {
                if (need_value(argc, argv, i) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
                if (set_width(opts, argv[++i]) != EXIT_SUCCESS)
                    return EXIT_FAILURE;
            } else if (!strcmp(arg, "--borg")) {
                apply_face_mode(opts, 'b');
            } else if (!strcmp(arg, "--dead")) {
                apply_face_mode(opts, 'd');
            } else if (!strcmp(arg, "--greedy")) {
                apply_face_mode(opts, 'g');
            } else if (!strcmp(arg, "--paranoid")) {
                apply_face_mode(opts, 'p');
            } else if (!strcmp(arg, "--stoned")) {
                apply_face_mode(opts, 's');
            } else if (!strcmp(arg, "--wired")) {
                apply_face_mode(opts, 'w');
            } else if (!strcmp(arg, "--young")) {
                apply_face_mode(opts, 'y');
            } else {
                fprintf(stderr, "ccowsay: unknown option '%s'\n", arg);
                return EXIT_FAILURE;
            }
            continue;
        }

        if (arg[2]) {
            fprintf(stderr, "ccowsay: unknown option '%s'\n", arg);
            return EXIT_FAILURE;
        }

        switch (arg[1]) {
        case 'h':
            usage(stdout);
            exit(EXIT_SUCCESS);
        case 'l':
            exit(list_cows(stdout));
        case 'f':
        case 'c':
            if (need_value(argc, argv, i) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            if (set_cow(opts, argv[++i]) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            break;
        case 'e':
            if (need_value(argc, argv, i) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            set_pair(opts->eyes, argv[++i]);
            break;
        case 'T':
            if (need_value(argc, argv, i) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            set_pair(opts->tongue, argv[++i]);
            break;
        case 't':
            opts->think = 1;
            break;
        case 'n':
            opts->no_wrap = 1;
            break;
        case 'W':
            if (need_value(argc, argv, i) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            if (set_width(opts, argv[++i]) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            break;
        case 'b':
        case 'd':
        case 'g':
        case 'p':
        case 's':
        case 'w':
        case 'y':
            apply_face_mode(opts, arg[1]);
            break;
        default:
            fprintf(stderr, "ccowsay: unknown option '%s'\n", arg);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    CcowsayOptions opts = {
        "oo",
        "  ",
        "default",
        0,
        0,
        40
    };
    Lines lines = {0};
    char* message = NULL;
    int result = EXIT_SUCCESS;

    if (parse_args(argc, argv, &opts, &message) != EXIT_SUCCESS) {
        usage(stderr);
        free(message);
        return EXIT_FAILURE;
    }

    if (!message && read_stdin_text(&message) != EXIT_SUCCESS) {
        fprintf(stderr, "ccowsay: out of memory\n");
        return EXIT_FAILURE;
    }

    if (!message || !message[0]) {
        usage(stderr);
        free(message);
        return EXIT_FAILURE;
    }

    if (split_or_wrap_text(message, opts.wrap_width, opts.no_wrap, &lines) != EXIT_SUCCESS) {
        fprintf(stderr, "ccowsay: out of memory\n");
        free(message);
        return EXIT_FAILURE;
    }

    print_balloon(stdout, &lines, opts.think);
    
    if (render_cow(stdout, opts.cow, &opts) != EXIT_SUCCESS) {
        free_lines(&lines);
        free(message);
        return EXIT_FAILURE;
    }

    free_lines(&lines);
    free(message);
    return result;
}
