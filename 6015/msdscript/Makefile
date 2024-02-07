# Makefile HW1 CS6015
# Aiden Pratt
# to build a target, enter make <target>
# targets: 
#clean deletes all .o and .out files and the executable
#printM prints the content of the MACROS
#run creates and runs the program in the terminal

CXX=c++
CFLAGS= -std=c++11
CXXSOURCE= cmdline.cpp main.cpp expr.cpp tests.cpp
HEADERS= cmdline.hpp catch.h expr.h


run: msdscript
	./msdscript

msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CFLAGS) -o msdscript $(CXXSOURCE)

.PHONY: clean

clean: 
	rm -f *.o *.out msdscript

test : msdscript
	./msdscript --test

printM:
	$ (echo $(HEADERS) $(CXXSOURCE))

doc: msdscript
	cd documentation && doxygen



