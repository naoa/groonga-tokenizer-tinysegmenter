AM_CFLAGS =					\
	$(GROONGA_CFLAGS)

AM_LDFLAGS =					\
	-avoid-version				\
	-module					\
	-no-undefined

LIBS =						\
	$(GROONGA_LIBS)

tokenizers_plugins_LTLIBRARIES =
tokenizers_plugins_LTLIBRARIES += tinysegmenter.la

tinysegmenter_la_SOURCES =			\
	tinysegmenter.cpp

tinysegmenter_la_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
tinysegmenter_la_LIBADD = $(LIBS)
tinysegmenter_la_LDFLAGS = $(AM_LDFLAGS)
