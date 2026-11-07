CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude

SRCS = src/orderbook.cpp src/signer.cpp src/client.cpp
OBJS = $(SRCS:.cpp=.o)

all: bin/delta_demo bin/run_tests

bin/delta_demo: $(OBJS) src/main.o
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/run_tests: $(OBJS) tests/test_delta.o
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: bin/run_tests
	./bin/run_tests

clean:
	rm -rf src/*.o tests/*.o bin

.PHONY: all test clean
