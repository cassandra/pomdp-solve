INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/mdp -I$(top_srcdir)/src/lp-solve  -I$(top_srcdir)/src/laspack

# Override this since it seems to want to do -g -O2 all the time whch
# seems to be the default and which I do not yet know how to change.
#
CFLAGS =

# Assumed location of dmalloc.h
#
DMALLOC_BASE = /usr/local

LDFLAGS = -L$(top_builddir)/src/mdp -L$(top_builddir)/src/lp-solve -L$(top_builddir)/src/laspack

COMMON_LIBS = $(top_builddir)/src/mdp/libmdp.a $(top_builddir)/src/lp-solve/liblpk.a $(top_builddir)/src/laspack/liblaspack.a @LEXLIB@

SUBDIRS = 

bin_PROGRAMS = pomdp-test

if DMALLOC

  ## DMALLOC options
  ##
  pomdp_test_CFLAGS = -g -DDMALLOC
  pomdp_test_INCLUDES = -I$(DMALLOC_BASE)/include
  pomdp_test_LDFLAGS = -L$(DMALLOC_BASE)/lib
  pomdp_test_LDADD = $(COMMON_LIBS) -ldmalloc

else

  pomdp_test_CFLAGS = -g
  pomdp_test_LDADD = $(COMMON_LIBS)

endif

############################################################
#####
##### pomdp-test
#####

COMMON = global-test.h ../src/global.c ../src/xalloc.c

INDEX_LIST = ../src/index-list.h ../src/index-list.c index-list-test.c 

DOUBLE_VECTOR = ../src/double-vector.h ../src/double-vector.c double-vector-test.c 

pomdp_test_SOURCES = $(COMMON) $(INDEX_LIST) $(DOUBLE_VECTOR) main-test.c

# For testing via 'make check'
##check_PROGRAMS = test-pomdp-solve
##test_pomdp_solve_SOURCES = ...
