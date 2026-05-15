AM_CPPFLAGS = \
	-DCCOWSAY_VERSION='"$(PACKAGE_VERSION)"' \
	-DDEFAULT_COWDIR='"$(pkgdatadir)/cows"'

bin_PROGRAMS = ccowsay
ccowsay_SOURCES = $(wildcard *.c)

cowdir = $(pkgdatadir)/cows
dist_cow_DATA = $(wildcard cows/*.cow)

dist_doc_DATA = README
