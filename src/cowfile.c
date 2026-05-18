#define _POSIX_C_SOURCE 200809L

#include "ccowsay.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int is_file(const char* path)
{
    struct stat st;

    if (!stat(path, &st) && S_ISREG(st.st_mode))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

static int has_suffix(const char* text, const char* suffix)
{
    size_t text_len = strlen(text);
    size_t suffix_len = strlen(suffix);

    if (text_len >= suffix_len
        && strcmp(text + text_len - suffix_len, suffix) == 0)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

static char* dup_slice(const char* start, size_t len)
{
    char* out = malloc(len + 1);

    if (!out)
        return NULL;

    memcpy(out, start, len);
    out[len] = '\0';
    return out;
}

static char* join_path(const char* dir, const char* name, const char* suffix)
{
    size_t dir_len = strlen(dir);
    size_t name_len = strlen(name);
    size_t suffix_len = strlen(suffix);

    const char* slash = (dir_len && dir[dir_len - 1] == '/') ? "" : "/";
    size_t path_len = dir_len + strlen(slash) + name_len + suffix_len + 1;
    
    char* path = malloc(path_len);

    if (!path)
        return NULL;

    snprintf(path, path_len, "%s%s%s%s", dir, slash, name, suffix);
    return path;
}

static const char* cowpath(void)
{
    const char* path = getenv("CCOWSAY_COWPATH");

    if (path && *path)
        return path;

    path = getenv("COWPATH");
    if (path && *path)
        return path;

    return DEFAULT_COWDIR;
}

static char* find_in_dir(const char* dir, const char* name)
{
    char* path = join_path(dir, name, "");

    if (!path)
        return NULL;
    if (is_file(path) == EXIT_SUCCESS)
        return path;

    free(path);
    if (has_suffix(name, ".cow") == EXIT_SUCCESS)
        return NULL;

    path = join_path(dir, name, ".cow");
    if (!path)
        return NULL;
    if (is_file(path) == EXIT_SUCCESS)
        return path;

    free(path);
    return NULL;
}

static char* resolve_cow(const char* name)
{
    char* paths;
    char* dir;
    char* saveptr = NULL;

    if (is_file(name) == EXIT_SUCCESS)
        return strdup(name);

    paths = strdup(cowpath());
    if (!paths)
        return NULL;

    for (dir = strtok_r(paths, ":", &saveptr); dir; dir = strtok_r(NULL, ":", &saveptr)) {
        char* found = find_in_dir(dir, name);

        if (found) {
            free(paths);
            return found;
        }
    }

    free(paths);
    return NULL;
}

static void print_template_line(FILE* out, const char* line, const CcowsayOptions* opts)
{
    const char* p = line;

    while (*p) {
        if (strncmp(p, "$eyes", 5) == 0) {
            fputs(opts->eyes, out);
            p += 5;
        } else if (strncmp(p, "$tongue", 7) == 0) {
            fputs(opts->tongue, out);
            p += 7;
        } else if (strncmp(p, "$thoughts", 9) == 0) {
            fputs(opts->think ? "o" : "\\", out);
            p += 9;
        } else {
            fputc(*p, out);
            p++;
        }
    }
}

int render_cow(FILE* out, const char* name, const CcowsayOptions* opts)
{
    char* path = resolve_cow(name);
    FILE* file;
    char* line = NULL;
    size_t cap = 0;
    ssize_t len;

    if (!path) {
        fprintf(stderr, "ccowsay: could not find cow '%s'\n", name);
        return EXIT_FAILURE;
    }

    file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "ccowsay: could not read '%s'\n", path);
        free(path);
        return EXIT_FAILURE;
    }

    while ((len = getline(&line, &cap, file)) != -1) {
        print_template_line(out, line, opts);
    }

    free(line);
    fclose(file);
    free(path);
    return EXIT_SUCCESS;
}

static int add_cow_name(CowList* cows, const char* file_name)
{
    char** next;
    size_t name_len = strlen(file_name) - strlen(".cow");
    char* name = dup_slice(file_name, name_len);

    if (!name)
        return EXIT_FAILURE;

    next = realloc(cows->names, sizeof(*next) * (cows->count + 1));
    if (!next) {
        free(name);
        return EXIT_FAILURE;
    }

    cows->names = next;
    cows->names[cows->count++] = name;
    return EXIT_SUCCESS;
}

static int collect_cows_in_dir(const char* dir, CowList* cows)
{
    DIR* dh = opendir(dir);
    struct dirent* entry;
    int result = EXIT_SUCCESS;

    if (!dh)
        return EXIT_SUCCESS;

    while ((entry = readdir(dh)) != NULL) {
        char* path;

        if (has_suffix(entry->d_name, ".cow") != EXIT_SUCCESS)
            continue;

        path = join_path(dir, entry->d_name, "");
        if (!path) {
            closedir(dh);
            return EXIT_FAILURE;
        }

        if (is_file(path) == EXIT_SUCCESS)
            result = add_cow_name(cows, entry->d_name);

        free(path);

        if (result != EXIT_SUCCESS)
            break;
    }

    closedir(dh);
    return result;
}

static int cmp_names(const void* a, const void* b)
{
    const char* left = *(const char* const*)a;
    const char* right = *(const char* const*)b;

    return strcmp(left, right);
}

int list_cows(FILE* out)
{
    char* paths = strdup(cowpath());
    char* dir;
    char* saveptr = NULL;
    CowList cows = {0};
    size_t i;
    int result = EXIT_SUCCESS;

    if (!paths) {
        fprintf(stderr, "ccowsay: out of memory\n");
        return EXIT_FAILURE;
    }

    for (dir = strtok_r(paths, ":", &saveptr); dir; dir = strtok_r(NULL, ":", &saveptr)) {
        if (collect_cows_in_dir(dir, &cows) != EXIT_SUCCESS) {
            result = EXIT_FAILURE;
            break;
        }
    }
    free(paths);

    if (result == EXIT_SUCCESS) {
        qsort(cows.names, cows.count, sizeof(*cows.names), cmp_names);
        for (i = 0; i < cows.count; i++) {
            if (i == 0 || strcmp(cows.names[i], cows.names[i - 1]) != 0)
                fprintf(out, "%s\n", cows.names[i]);
        }
    }

    for (i = 0; i < cows.count; i++)
        free(cows.names[i]);
    free(cows.names);

    if (result != EXIT_SUCCESS)
        fprintf(stderr, "ccowsay: out of memory\n");

    return result;
}
