#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccowsay.h"

#suite cows
#tcase registry
#test cow_registry_is_not_empty
    size_t count = 0;
    const ccowsay_cow *cows = ccowsay_cows(&count);
    ck_assert_ptr_nonnull(cows);
    ck_assert_uint_ge(count, 5);

#test all_cows_have_names_and_templates
    size_t count = 0;
    const ccowsay_cow *cows = ccowsay_cows(&count);
    for (size_t i = 0; i < count; i++) {
        ck_assert_ptr_nonnull(cows[i].name);
        ck_assert_str_ne(cows[i].name, "");
        ck_assert_ptr_nonnull(cows[i].template);
        ck_assert_str_ne(cows[i].template, "");
        ck_assert_ptr_eq(ccowsay_find_cow(cows[i].name), &cows[i]);
    }

#test unknown_cow_is_not_found
    ck_assert_ptr_null(ccowsay_find_cow("definitely-not-a-cow"));

#tcase render
#test default_cow_renders_cowsay_markers
    char *buffer = NULL;
    size_t size = 0;
    FILE *stream = open_memstream(&buffer, &size);
    ck_assert_ptr_nonnull(stream);
    ccowsay_print_cow(stream, "default", 'o', '\\', '\\');
    fclose(stream);
    ck_assert_ptr_nonnull(strstr(buffer, "^__^"));
    ck_assert_ptr_nonnull(strstr(buffer, "(oo)"));
    free(buffer);

#test default_cow_renders_cowthink_markers
    char *buffer = NULL;
    size_t size = 0;
    FILE *stream = open_memstream(&buffer, &size);
    ck_assert_ptr_nonnull(stream);
    ccowsay_print_cow(stream, "default", 'o', 'O', 'o');
    fclose(stream);
    ck_assert_ptr_nonnull(strstr(buffer, "O   ^__^"));
    ck_assert_ptr_nonnull(strstr(buffer, "o  (oo)"));
    free(buffer);
