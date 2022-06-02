export CK_FORK=no

TESTS = check_ynot_c_ds

@VALGRIND_CHECK_RULES@

check_PROGRAMS = check_ynot_c_ds
check_ynot_c_ds_SOURCES =   check_array.c     \
							check_list.c      \
							check_map.c       \
							check_queue.c     \
							check_pqueue.c    \
							check_ptrie.c     \
							check_ynot_c_ds.h \
							check_ynot_c_ds.c \
							$(top_srcdir)/src/ynot_c_ds.h
check_ynot_c_ds_CFLAGS = @CHECK_CFLAGS@
check_ynot_c_ds_CPPFLAGS = -I$(top_srcdir)/src
check_ynot_c_ds_LDADD = $(top_builddir)/src/libynot_c_ds.la @CHECK_LIBS@
check_ynot_c_ds_LDFLAGS = -no-undefined
