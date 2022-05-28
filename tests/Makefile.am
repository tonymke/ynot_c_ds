TESTS = check_ynot_c_ds
check_PROGRAMS = check_ynot_c_ds
check_ynot_c_ds_SOURCES =                   \
						  check_array.c     \
						  check_list.c      \
						  check_map.c       \
						  check_ynot_c_ds.h \
						  check_ynot_c_ds.c \
						  ynot_c_ds.h
check_ynot_c_ds_CFLAGS = @CHECK_CFLAGS@
check_ynot_c_ds_CPPFLAGS = -I$(top_builddir)/src
check_ynot_c_ds_LDADD = $(top_builddir)/src/libynot_c_ds.la @CHECK_LIBS@
check_ynot_c_ds_LDFLAGS = -no-undefined
