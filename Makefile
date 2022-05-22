CFLAGS := -ansi -Wall -Wextra -Werror -pedantic -pedantic-errors

DISTS := libynot_c_ds.so
SRC := $(filter-out %_check.c, $(wildcard *.c))
OBJS := $(patsubst %.c,%.o,$(SRC))

CHECK_SRC := $(filter %_check.c, $(wildcard *.c))
CHECK_BINS := $(patsubst %.c,%,$(CHECK_SRC))

.PHONY: all check clean

all: $(DISTS)

check: $(CHECK_BINS)
	@for bin in $^; do \
		printf "$$bin: "; \
		if $(dir $$bin)$(notdir $$bin); then \
			printf "PASS %d\n" $$?; \
		else \
			printf "FAIL %d\n" $$?; \
			exit 1; \
		fi \
	done

clean:
	rm -f $(CHECK_BINS) $(DISTS) *.o *.d *.exe

include $(SRC:.c=.d)
include $(CHECK_SRC:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%_check: LDLIBS = -lcheck
%_check: %_check.o $(OBJS)
	$(CC) $(CHECK_LD_FLAGS) -o  $@ $^ $(LOADLIBES) $(LDLIBS)

%.so: LDFLAGS = -shared
%.so: $(OBJS)
	$(CC) $(LDFLAGS) -o  $@ $^ $(LOADLIBES) $(LDLIBS)
