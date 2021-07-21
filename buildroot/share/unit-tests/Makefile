#
# Makefile for Marlin Unit Tests
#

TEST_SOURCES := $(wildcard ./*.tests.cpp)
TEST_OUT := $(TEST_SOURCES:%.cpp=%.ut)
INC_TEST_FILES := $(addprefix echo "Testing ", $(addsuffix ";", $(TEST_SOURCES:%.cpp=%.ut)))
RUN_TESTS := $(foreach test, $(TEST_SOURCES:%.cpp=%.ut), echo -e "Testing $(test)"; /bin/sh -c "$(test)";)

unitTest: TestVector.hpp TestVector.cpp $(TEST_OUT)
	@$(RUN_TESTS)

%.tests.ut : %.tests.cpp
	@echo "Building: $<"
	@$(CXX) -std=c++14 -O0 -ggdb3 -include TestVector.hpp -include $< TestVector.cpp -o $@

clean:
	@-rm $(TEST_OUT) 2>/dev/null
