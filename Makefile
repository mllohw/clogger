#   Makefile
#
#   Copyright (C) 2025  Mirko Lindroth
#   SPDX-License-Identifier: MIT

# Directory settings
makefile_path    := $(abspath $(firstword $(MAKEFILE_LIST)))
makefile_dir     := $(patsubst %/,%,$(dir $(makefile_path)))
src_dir          := $(makefile_dir)/src
test_dir         := $(makefile_dir)/tests
example_dir      := $(makefile_dir)/examples
include_dir      := $(makefile_dir)/include
bin_dir          := $(makefile_dir)/bin

# Lib sources
src := $(shell find $(src_dir) -type f -name "*.c" -o -name "*.h")
objects := $(patsubst %.c, %.o, $(filter %.c,$(src)))

# Test sources
test_src := $(shell find $(test_dir) -type f -name "*.c" -o -name "*.h")
test_objects := $(patsubst %.c, %.o, $(filter %.c,$(test_src)))
test_bins := $(patsubst %.c, $(bin_dir)/$(basename %), \
		$(filter %.c,$(notdir $(test_src))))

# Example sources
example_src := $(shell find $(example_dir) -type f -name "*.c" -o -name "*.h")
example_objects := $(patsubst %.c, %.o, $(filter %.c,$(example_src)))
example_bins := $(patsubst %.c, $(bin_dir)/$(basename %), \
		$(filter %.c,$(notdir $(example_src))))

# Recipe settings
SHELL         := bash
CC            := gcc
CFLAGS        := -I $(include_dir)
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
test: $(test_bins)
	$<

.PHONY: examples
#: Compiles examples
examples: $(example_bins)

.PHONY: clean
#: Clean all output files
clean: mostlyclean
	rm -rf $(bin_dir)

.PHONY: mostlyclean
#: Clean object files and logs
mostlyclean:
	rm -rf $(objects)
	rm -rf $(test_objects)
	rm -rf $(example_objects)
	rm -rf *.log

$(test_bins): $(test_objects) $(objects)
	@mkdir -p $(@D)
	gcc $^ -o $@ -lm

$(example_bins): $(example_objects) $(objects)
	@mkdir -p $(@D)
	gcc $^ -o $@ -lm

$(objects): $(src)

$(test_objects): $(test_src)

$(example_objects): $(exmple_src)
