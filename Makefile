GIT_HOOK := .git/hooks/applied
$(GIT_HOOK): scripts/install-git-hooks
	@$<
	@echo

.PHONY: all check clean
all: $(GIT_HOOK) check
.DEFAULT_GOAL := all

include common.mk

CFLAGS = -I./include
CFLAGS += -std=c99 -pedantic -Wall -W -Werror

TESTS = \
    containerof \
    list_entry \
    list_init-explicit \
    list_init-local \
    list_init-global \
    list_add \
    list_add_tail \
    list_del \
    list_del_init \
    list_first_entry \
    list_last_entry \
    list_is_singular \
    list_for_each_safe \
    list_for_each_entry_safe \
    list_for_each \
    list_for_each_entry \
    list_move \
    list_move_tail \
    list_splice \
    list_splice_tail \
    list_splice_init \
    list_splice_tail_init \
    list_cut_position

TESTS := $(addprefix tests/,$(TESTS))
# dependency of source files
deps := $(TESTS:%=%.o.d)

TESTS_OK = $(TESTS:=.ok)

check: $(TESTS_OK)

$(TESTS_OK): %.ok: %
	$(Q)$(PRINTF) "*** Validating $< ***\n"
	$(Q)./$< && $(PRINTF) "\t$(PASS_COLOR)[ Verified ]$(NO_COLOR)\n"
	@touch $@

# standard build rules
.SUFFIXES: .o .c
.c.o:
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

$(TESTS): %: %.o
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) -o $@ $^ $(LDFLAGS)

MERGESORT = \
	merge-sort \

QUICKSORT = \
	quick-sort \

INSERTSORT = \
	insert-sort \

MERGESORT := $(addprefix examples/,$(MERGESORT))
QUICKSORT := $(addprefix examples/,$(QUICKSORT))
INSERTSORT := $(addprefix examples/,$(INSERTSORT))


merge-sort: $(MERGESORT)
	@$< 10000
	@$< 20000
	@$< 30000
	@$< 40000
	@$< 50000

quick-sort: $(QUICKSORT)
	@$< 10000
	@$< 20000
	@$< 30000
	@$< 40000
	@$< 50000

insert-sort: $(INSERTSORT)
	@$< 10000
	@$< 20000
	@$< 30000
	@$< 40000
	@$< 50000

$(SORTS): %: %.c
	$(CC) -o $@ -I./include $(LDFLAGS) $<

clean:
	$(VECHO) "  Cleaning...\n"
	$(Q)$(RM) $(TESTS) $(TESTS_OK) $(TESTS:=.o) $(deps)

-include $(deps)
