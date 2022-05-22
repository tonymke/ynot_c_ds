CFLAGS := -g -ansi -Wall -Wextra -Werror -pedantic -pedantic-errors

SRC := $(filter-out %_check.c, $(wildcard *.c))
OBJS := $(patsubst %.c,%.o,$(SRC))

CHECK_SRC := $(filter %_check.c, $(wildcard *.c))
CHECK_OBJS := $(patsubst %.c,%.o,$(CHECK_SRC))

ifeq ($(OS),Windows_NT)
	DIST_LIBS := ynot_c_ds.dll
	CHECK_BIN := ynot_c_ds_check.exe
else
	DIST_LIBS := libynot_c_ds.so
	CHECK_BIN := ynot_c_ds_check.exe
endif

.PHONY: all check clean superclean

all: $(DIST_LIBS)

check: $(CHECK_BIN)
	$(dir $<)$(notdir $<)

clean:
	rm -f $(OBJS) $(DIST_LIBS) $(CHECK_OBJS) $(CHECK_BINS) \
		$(patsubst %.o,%.d,$(CHECK_OBJS)) $(patsubst %.o,%.d,$(OBJS))

superclean:
	rm -f $(CHECK_BINS) *.{o,d,exe,so,dll}

include $(SRC:.c=.d)
include $(CHECK_SRC:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%_check %_check.exe: LDFLAGS += -L.
%_check %_check.exe: LDLIBS += -lcheck -lynot_c_ds
%_check %_check.exe: $(CHECK_OBJS) $(DIST_LIBS)
	$(CC) $(LDFLAGS) -o  $@ $(filter %.o,$^) $(LOADLIBES) $(LDLIBS)

%.so %.dll: LDFLAGS += -shared
%.so %.dll: $(OBJS)
	$(CC) $(LDFLAGS) -o  $@ $^ $(LOADLIBES) $(LDLIBS)
