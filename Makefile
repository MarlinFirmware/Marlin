SCRIPTS_DIR := buildroot/share/scripts
CONTAINER_RT_BIN := docker
CONTAINER_RT_OPTS := --rm -v $(PWD):/code -v platformio-cache:/root/.platformio
CONTAINER_IMAGE := marlin-dev

help:
	@echo "Tasks for local development:"
	@echo "make marlin                    : Build marlin for the configured board"
	@echo "make format-pins               : Reformat all pins files"
	@echo "make tests-single-ci           : Run a single test from inside the CI"
	@echo "make tests-single-local        : Run a single test locally"
	@echo "make tests-single-local-docker : Run a single test locally, using docker"
	@echo "make tests-all-local           : Run all tests locally"
	@echo "make tests-all-local-docker    : Run all tests locally, using docker"
	@echo "make tests-code-single-ci      : Run a single code test from inside the CI"
	@echo "make tests-code-single-local   : Run a single code test locally"
	@echo "make tests-code-single-local-docker : Run a single code test locally, using docker-compose"
	@echo "make tests-code-all-local      : Run all code tests locally"
	@echo "make tests-code-all-local-docker : Run all code tests locally, using docker-compose"
	@echo "make setup-local-docker        : Setup local docker-compose"
	@echo ""
	@echo "Options for testing:"
	@echo "  TEST_TARGET          Set when running tests-single-*, to select the"
	@echo "                       test. If you set it to ALL it will run all "
	@echo "                       tests, but some of them are broken: use "
	@echo "                       tests-all-* instead to run only the ones that "
	@echo "                       run on GitHub CI"
	@echo "  ONLY_TEST            Limit tests to only those that contain this, or"
	@echo "                       the index of the test (1-based)"
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
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS) $(CONTAINER_IMAGE) $(MAKE) tests-single-local TEST_TARGET=$(TEST_TARGET) VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD) ONLY_TEST="$(ONLY_TEST)"

tests-all-local:
	export PATH="./buildroot/bin/:./buildroot/tests/:${PATH}" \
	  && export VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) \
	  && for TEST_TARGET in $$($(SCRIPTS_DIR)/get_test_targets.py) ; do echo "Running tests for $$TEST_TARGET" ; run_tests . $$TEST_TARGET ; done

tests-all-local-docker:
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run $(CONTAINER_RT_OPTS) $(CONTAINER_IMAGE) $(MAKE) tests-all-local VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD)

tests-code-single-ci:
	export GIT_RESET_HARD=true
	$(MAKE) tests-code-single-local TEST_TARGET=$(TEST_TARGET)
.PHONY: tests-code-single-ci

# TODO: How can we limit tests with ONLY_TEST with platformio?
tests-code-single-local:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-code-all-local" ; return 1; fi
	platformio run -t marlin_$(TEST_TARGET)
.PHONY: tests-code-single-local

tests-code-single-local-docker:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-code-all-local-docker" ; return 1; fi
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run --rm marlin $(MAKE) tests-code-single-local TEST_TARGET=$(TEST_TARGET) ONLY_TEST="$(ONLY_TEST)"
.PHONY: tests-code-single-local-docker

tests-code-all-local:
	platformio run -t test-marlin
.PHONY: tests-code-all-local

tests-code-all-local-docker:
	@if ! $(CONTAINER_RT_BIN) images -q $(CONTAINER_IMAGE) > /dev/null ; then $(MAKE) setup-local-docker ; fi
	$(CONTAINER_RT_BIN) run --rm marlin $(MAKE) tests-code-all-local
.PHONY: tests-code-all-local-dockerS

setup-local-docker:
	$(CONTAINER_RT_BIN) build -t $(CONTAINER_IMAGE) -f docker/Dockerfile .

PINS := $(shell find Marlin/src/pins -mindepth 2 -name '*.h')

$(PINS): %:
	@echo "Formatting $@" && node $(SCRIPTS_DIR)/pinsformat.js $@

format-pins: $(PINS)
