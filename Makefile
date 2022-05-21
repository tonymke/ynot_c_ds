CFLAGS := -ansi -Wall -Wextra -Werror -pedantic -pedantic-errors

TARGET_LIBS := libynot_c_ds.so
CHECK_BINS := list_check
CHECK_SRC := $(filter %_check.c, $(wildcard *.c))
SRC := $(filter-out %_check.c, $(wildcard *.c))
OBJS := $(patsubst %.c,%.o,$(SRC))
CHECK_OBJS := $(patsubst %.c,%.o,$(CHECK_SRC))

.PHONY: all check clean

all: $(TARGET_LIBS)

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
	rm -f $(CHECK_BINS) $(TARGET_LIBS) *.o *.d *.exe

include $(SRC:.c=.d)
include $(CHECK_SRC:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(CHECK_BINS): $(OBJS) $(CHECK_OBJS)

$(TARGET_LIBS): $(OBJS)
	$(CC) $(LDFLAGS) -shared -o  $@ $^ $(LOADLIBES) $(LDLIBS)
