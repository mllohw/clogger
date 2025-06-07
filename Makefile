#   Makefile
#
#   Copyright (C) 2025  Mirko Lindroth
#   SPDX-License-Identifier: MIT

# Directory settings
makefile_path    := $(abspath $(firstword $(MAKEFILE_LIST)))
makefile_dir     := $(patsubst %/,%,$(dir $(makefile_path)))
src_dir          := $(makefile_dir)/src
test_dir         := $(makefile_dir)/tests

# Output app binary
exe := clogger

# App sources
src := $(shell find $(src_dir) -type f -name "*.c" -o -name "*.h")
includes := $(sort $(dir $(wildcard $(src_dir)/*/)))
objects := $(patsubst %.c, %.o, $(filter %.c,$(src)))

# Test sources
test_src := $(shell find $(test_dir) -type f -name "*.c" -o -name "*.h")
test_includes := $(sort $(dir $(wildcard $(test_dir)/*/)))
test_objects := $(patsubst %.c, %.o, $(filter %.c,$(test_src)))

CC := gcc

# Recipe settings
SHELL         := bash
.DEFAULT_GOAL := help

.PHONY: help
#: Prints available phony targets
help:
	@for makefile in $(MAKEFILE_LIST); do \
		echo -e "\n$$makefile targets:"; \
		echo -e "-------------------------------"; \
		grep -B1 -E "^[a-zA-Z0-9_-]+:([^\=]|$$)" $$makefile \
			| sed '/^[#||-]/! s/[^:]*$$//' \
			| uniq \
			| grep -v -- -- \
			| sed 'N;s/\n/###/' \
			| sed -n 's/^#: \(.*\)###\(.*\):.*/\2###\1/p' \
			| column -ts '###' \
			| sort; \
	done

.PHONY: test
#: Run tests
test: run_test
	./$<

run_test: $(test_objects) $(objects)
	gcc $^ -o $@ -lm

.PHONY: comp
#: Compile application
comp: $(exe)
$(exe): $(objects)
	gcc $^ -o $@

.PHONY: clean
#: Clean all output files
clean: mostlyclean
	rm -rf $(exe)
	rm -rf run_test

.PHONY: mostlyclean
#: Clean object files and logs
mostlyclean:
	rm -rf $(objects)
	rm -rf $(test_objects)
	rm -rf *.log

$(objects): $(src)

$(test_objects): $(test_src)
