CFLAGS := -g -ansi -Wall -Wextra -Werror -pedantic -pedantic-errors

SRC := $(filter-out %_check.c, $(wildcard *.c))
OBJS := $(patsubst %.c,%.o,$(SRC))

CHECK_SRC := $(filter %_check.c, $(wildcard *.c))
CHECK_OBJS := $(patsubst %.c,%.o,$(CHECK_SRC))
CHECK_BINS := $(patsubst %.c,%,$(CHECK_SRC))

ifeq ($(OS),Windows_NT)
	DIST_LIBS := ynot_c_ds.dll
	CHECK_BINS := $(patsubst %.c,%.exe,$(CHECK_SRC))
else
	DIST_LIBS := libynot_c_ds.so
	CHECK_BINS := $(patsubst %.c,%,$(CHECK_SRC))
endif

.PHONY: all check clean superclean

all: $(DIST_LIBS)

check: $(CHECK_BINS)
	$(foreach check_bin, $^, $(dir $(check_bin))$(notdir $(check_bin));)

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
%_check %_check.exe: %_check.o $(DIST_LIBS)
	$(CC) $(LDFLAGS) -o  $@ $(filter %.o,$^) $(LOADLIBES) $(LDLIBS)

%.so %.dll: LDFLAGS += -shared
%.so %.dll: $(OBJS)
	$(CC) $(LDFLAGS) -o  $@ $^ $(LOADLIBES) $(LDLIBS)
