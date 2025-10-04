SCRIPTS_DIR := buildroot/share/scripts
CONTAINER_RT_BIN := docker
CONTAINER_RT_OPTS := --rm -v $(PWD):/code -v platformio-cache:/root/.platformio
CONTAINER_IMAGE := marlin-dev
UNIT_TEST_CONFIG ?= default

help:
	@echo "Tasks for local development:"
	@echo "make marlin                    : Build Marlin for the configured board"
	@echo "make format-pins -j            : Reformat all pins files (-j for parallel execution)"
	@echo "make validate-lines -j         : Validate line endings, fails on trailing whitespace, etc."
	@echo "make validate-pins -j          : Validate all pins files, fails if any require reformatting"
	@echo "make validate-boards -j        : Validate boards.h and pins.h for standards compliance"
	@echo "make tests-single-ci           : Run a single test from inside the CI"
	@echo "make tests-single-local        : Run a single test locally"
	@echo "make tests-single-local-docker : Run a single test locally, using docker"
	@echo "make tests-all-local           : Run all tests locally"
	@echo "make tests-all-local-docker    : Run all tests locally, using docker"
	@echo "make unit-test-single-local    : Run unit tests for a single config locally"
	@echo "make unit-test-single-local-docker : Run unit tests for a single config locally, using docker"
	@echo "make unit-test-all-local       : Run all code tests locally"
	@echo "make unit-test-all-local-docker : Run all code tests locally, using docker"
	@echo "make setup-local-docker        : Setup local docker using buildx"
	@echo ""
	@echo "Options for testing:"
	@echo "  TEST_TARGET          Set when running tests-single-*, to select the"
	@echo "                       test. If you set it to ALL it will run all "
	@echo "                       tests, but some of them are broken: use "
	@echo "                       tests-all-* instead to run only the ones that "
	@echo "                       run on GitHub CI"
	@echo "  ONLY_TEST            Limit tests to only those that contain this, or"
	@echo "                       the index of the test (1-based)"
	@echo "  UNIT_TEST_CONFIG     Set the name of the config from the test folder, without"
	@echo "                       the leading number. Default is 'default'". Used with the
	@echo "                       unit-test-single-* tasks"
	@echo "  VERBOSE_PLATFORMIO   If you want the full PIO output, set any value"
	@echo "  GIT_RESET_HARD       Used by CI: reset all local changes. WARNING:"
	@echo "                       THIS WILL UNDO ANY CHANGES YOU'VE MADE!"

marlin:
	./buildroot/bin/mftest -a
.PHONY: marlin

tests-single-ci:
	export GIT_RESET_HARD=true
	$(MAKE) tests-single-local TEST_TARGET=$(TEST_TARGET) PLATFORMIO_BUILD_FLAGS=-DGITHUB_ACTION

tests-single-local:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-all-local" ; return 1; fi
	export PATH="./buildroot/bin/:./buildroot/tests/:${PATH}" \
	  && export VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) \
	  && run_tests . $(TEST_TARGET) "$(ONLY_TEST)"

tests-single-local-docker:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-all-local-docker" ; return 1; fi
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS) $(CONTAINER_IMAGE) make tests-single-local TEST_TARGET=$(TEST_TARGET) VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD) ONLY_TEST="$(ONLY_TEST)"

tests-all-local:
	@$(PYTHON) -c "import yaml" 2>/dev/null || (echo 'pyyaml module is not installed. Install it with "$(PYTHON) -m pip install pyyaml"' && exit 1)
	export PATH="./buildroot/bin/:./buildroot/tests/:${PATH}" \
	  && export VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) \
	  && for TEST_TARGET in $$($(PYTHON) $(SCRIPTS_DIR)/get_test_targets.py) ; do \
	    if [ "$$TEST_TARGET" = "linux_native" ] && [ "$$(uname)" = "Darwin" ]; then \
	      echo "Skipping tests for $$TEST_TARGET on macOS" ; \
	      continue ; \
	    fi ; \
	    echo "Running tests for $$TEST_TARGET" ; \
	    run_tests . $$TEST_TARGET || exit 1 ; \
	    sleep 5; \
	  done

tests-all-local-docker:
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS) $(CONTAINER_IMAGE) make tests-all-local VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD)

unit-test-single-local:
	platformio run -t marlin_$(UNIT_TEST_CONFIG) -e linux_native_test

unit-test-single-local-docker:
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS)  $(CONTAINER_IMAGE) make unit-test-single-local UNIT_TEST_CONFIG=$(UNIT_TEST_CONFIG)

unit-test-all-local:
	platformio run -t test-marlin -e linux_native_test

unit-test-all-local-docker:
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS)  $(CONTAINER_IMAGE) make unit-test-all-local

setup-local-docker:
	$(CONTAINER_RT_BIN) buildx build -t $(CONTAINER_IMAGE) -f docker/Dockerfile .

# Cross-platform file finding and path handling
ifeq ($(OS),Windows_NT)
    # Windows: Use cmd tools to find pins files
    PINS_RAW := $(shell cmd //c "dir /s /b Marlin\src\pins\*.h 2>nul | findstr /r ".*Marlin\\\\src\\\\pins\\\\.*\\\\pins_.*\.h"")
    PINS := $(subst \,/,$(PINS_RAW))
    # Windows: Check for available Python executable (try python, then python3, then py)
    PYTHON := $(shell where python 2>nul)
    ifeq ($(PYTHON),)
        PYTHON := $(shell where python3 2>nul)
        ifeq ($(PYTHON),)
            PYTHON := $(shell where py 2>nul)
            ifeq ($(PYTHON),)
                $(error No Python executable found. Please install Python 3.x from python.org or Microsoft Store)
            else
                # Verify py launcher points to Python 3
                PYTHON_VERSION := $(shell py -c "import sys; print(sys.version_info[0])" 2>nul)
                ifneq ($(PYTHON_VERSION),3)
                    $(error Found py launcher but it's not configured for Python 3. Use 'py -3' or install Python 3.x)
                endif
                PYTHON := py
            endif
        else
            # Verify python3 is actually Python 3
            PYTHON_VERSION := $(shell python3 -c "import sys; print(sys.version_info[0])" 2>nul)
            ifneq ($(PYTHON_VERSION),3)
                $(error Found python3 but it's not Python 3. Please reinstall Python 3.x)
            endif
            PYTHON := python3
        endif
    else
        # Verify python is actually Python 3
        PYTHON_VERSION := $(shell python -c "import sys; print(sys.version_info[0])" 2>nul)
        ifneq ($(PYTHON_VERSION),3)
            $(error Found python but it's not Python 3. Please install Python 3.x or use 'python3')
        endif
        PYTHON := python
    endif
else
    # Unix/Linux: Use find command
    PINS := $(shell find Marlin/src/pins -mindepth 2 -name 'pins_*.h')
    # Check for available Python executable (prefer python3, fallback to python)
    PYTHON := $(shell command -v python3 2>/dev/null)
    ifeq ($(PYTHON),)
        PYTHON := $(shell command -v python 2>/dev/null)
        ifeq ($(PYTHON),)
            $(error No Python executable found. Please install Python 3.x)
        else
            # Verify it's Python 3
            PYTHON_VERSION := $(shell python -c "import sys; print(sys.version_info[0])" 2>/dev/null)
            ifneq ($(PYTHON_VERSION),3)
                $(error Found python but it's not Python 3. Please install Python 3.x or ensure python3 is in PATH)
            endif
            PYTHON := python
        endif
    else
        PYTHON := python3
    endif
endif

.PHONY: $(PINS) format-pins validate-pins

$(PINS): %:
	@echo "Formatting pins $@"
	@$(PYTHON) $(SCRIPTS_DIR)/pinsformat.py $< $@

format-pins: $(PINS)
	@echo "Found $(words $(PINS)) pins files to format"

validate-pins: format-pins
	@echo "Validating pins files"
	@git diff --exit-code || (git status && echo "\nError: Pins files are not formatted correctly. Run \"make format-pins\" to fix.\n" && exit 1)

.PHONY: format-lines validate-lines

format-lines:
	@echo "Formatting all sources"
	@$(PYTHON) $(SCRIPTS_DIR)/linesformat.py buildroot
	@$(PYTHON) $(SCRIPTS_DIR)/linesformat.py Marlin

validate-lines:
	@echo "Validating text formatting"
	@npx prettier --check . --editorconfig --object-wrap preserve

BOARDS_FILE := Marlin/src/core/boards.h

.PHONY: validate-boards

validate-boards:
	@echo "Validating boards.h file"
	@$(PYTHON) $(SCRIPTS_DIR)/validate_boards.py $(BOARDS_FILE) || (echo "\nError: boards.h file is not valid. Please check and correct it.\n" && exit 1)
