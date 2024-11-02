CC = gcc -m32
CXX = g++ -m32
CFLAGS = -g -O2
CXXFLAGS = -g -O2

JWASM = ../../jw/jwasm

all: main

main: main.o block.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp picosha2.h
block.o: block.asm
	$(JWASM) -elf $<
