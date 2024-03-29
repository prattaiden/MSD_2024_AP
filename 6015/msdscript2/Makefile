# Makefile HW1 CS6015
# Aiden Pratt
# to build a target, enter make <target>
# targets: 
#clean deletes all .o and .out files and the executable
#printM prints the content of the MACROS
#run creates and runs the program in the terminal

CXX=c++
CFLAGS= -std=c++11
CXXSOURCE= cmdline.cpp main.cpp expr.cpp tests.cpp parse.cpp Val.cpp Env.cpp
HEADERS= cmdline.hpp catch.h expr.h parse.hpp Val.h pointer.h Env.h


run: msdscript
	./msdscript

msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CFLAGS) -o msdscript $(CXXSOURCE)

.PHONY: clean

clean: 
	rm -f *.o *.out msdscript

test : msdscript
	./msdscript --test

interp: msdscript
	./msdscript --interp

printM:
	$ (echo $(HEADERS) $(CXXSOURCE))

doc: msdscript
	cd documentation && doxygen

test_msdscript : test_msdscript.cpp exec.cpp expr.h
	$(CXX) $(CFLAGS) test_msdscript.cpp exec.cpp -o tester



