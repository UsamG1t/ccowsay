ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-DCCOWSAY_VERSION='"$(PACKAGE_VERSION)"' \
	-DDEFAULT_COWDIR='"$(pkgdatadir)/cows"'

lib_LTLIBRARIES = libccowsay.la
libccowsay_la_SOURCES = $(wildcard lib/*.c)
include_HEADERS = $(wildcard lib/*.h)

bin_PROGRAMS = ccowsay cowthink
ccowsay_SOURCES = $(wildcard src/*.c)
ccowsay_LDADD = libccowsay.la
cowthink_SOURCES = $(ccowsay_SOURCES)
cowthink_LDADD = libccowsay.la

cowdir = $(pkgdatadir)/cows
dist_cow_DATA = $(wildcard cows/*.cow)

dist_doc_DATA = README COWSOURCES.md

TESTS = tests/cows tests/cow-utilities.sh
check_PROGRAMS = tests/cows

.ts.c:
	checkmk $< > $@

AM_CFLAGS = -D_GNU_SOURCE @CHECK_CFLAGS@
LDADD = libccowsay.la @CHECK_LIBS@
tests_cows_SOURCES = tests/cows.ts
EXTRA_DIST = $(wildcard tests/*.sh) tests/cows.ts
