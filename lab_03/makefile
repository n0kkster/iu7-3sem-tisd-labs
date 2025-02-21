BASEDIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
SRCDIR := $(BASEDIR)/src
INCDIR := $(BASEDIR)/inc
OBJDIR := $(BASEDIR)/out
UNITDIR := $(BASEDIR)/unit_tests

vpath %.c $(SRCDIR) $(UNITDIR)

CC := gcc
CLANG := clang

CFLAGS := -std=c99 -Werror -Wall -Wextra -Wfloat-equal -Wfloat-conversion -Wpedantic -I$(INCDIR)
UNITFLAGS := -lcheck -lpthread -lrt
LINK_FLAGS := -lm

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
UNIT_SRC := $(wildcard $(UNITDIR)/*.c)
UNIT_OBJ := $(UNIT_SRC:$(UNITDIR)/%.c=$(OBJDIR)/%.o)

DEBUG_FLAGS := -O0 -g
RELEASE_FLAGS := -O2
COVERAGE_FLAGS := --coverage
ASAN_FLAGS := -fsanitize=address -fno-omit-frame-pointer
MSAN_FLAGS := -fsanitize=memory -fPIE -fno-omit-frame-pointer
UBSAN_FLAGS := -fsanitize=undefined -fno-omit-frame-pointer

release: .release.flag
release: CFLAGS += $(RELEASE_FLAGS)
release: app.exe

debug: .debug.flag
debug: CFLAGS += $(DEBUG_FLAGS)
debug: app.exe

coverage: .coverage.flag
coverage: CFLAGS += $(DEBUG_FLAGS) $(COVERAGE_FLAGS)
coverage: LINK_FLAGS += --coverage
coverage: app.exe

asan: .asan.flag
asan: CFLAGS += $(DEBUG_FLAGS) $(ASAN_FLAGS)
asan: LINK_FLAGS += -fsanitize=address
asan: CC = $(CLANG)
asan: app.exe

msan: .msan.flag
msan: CFLAGS += $(DEBUG_FLAGS) $(MSAN_FLAGS)
msan: LINK_FLAGS += -fsanitize=memory -pie
msan: CC = $(CLANG)
msan: app.exe

ubsan: .ubsan.flag
ubsan: CFLAGS += $(DEBUG_FLAGS) $(UBSAN_FLAGS)
ubsan: LINK_FLAGS += -fsanitize=undefined
ubsan: CC = $(CLANG)
ubsan: app.exe

unit: unit_tests.exe
	$(BASEDIR)/unit_tests.exe

func: app.exe
	$(BASEDIR)/func_tests/scripts/func_tests.sh -v 

app.exe: $(OBJ)
	$(CC) $^ $(LINK_FLAGS) -o $(BASEDIR)/$@

unit_tests.exe: $(UNIT_OBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ))
	$(CC) $^ $(LINK_FLAGS) $(UNITFLAGS) -o $(BASEDIR)/$@

include $(OBJ:.o=.d)
include $(UNITOBJ:.o=.d)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM -MT $@ $< > $@

clean:
	rm -rf $(OBJDIR)/*
	rm -f $(BASEDIR)/func_tests/data/*.tmp
	rm -f $(BASEDIR)/*.exe
	find $(BASEDIR) -iname *.flag -delete

.%.flag:
	rm -f $(OBJDIR)/*.o
	find $(BASEDIR) -iname *.flag -delete
	touch $(BASEDIR)/$@

.PHONY: release debug asan msan ubsan coverage unit func clean
