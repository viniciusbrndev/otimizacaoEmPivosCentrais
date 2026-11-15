CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude
LIB_SRC  := $(filter-out src/main.cpp,$(wildcard src/*.cpp))

.PHONY: all teste clean

all: pivos

pivos: src/main.cpp $(LIB_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

teste: tests/teste_consistencia.cpp $(LIB_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	./$@

clean:
	rm -f pivos teste
