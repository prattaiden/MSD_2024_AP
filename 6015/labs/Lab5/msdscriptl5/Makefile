# Makefile HW1 CS6015
# Aiden Pratt
# to build a target, enter make <target>
# targets: 
# clean deletes all .o and .out files and the executable
# printM prints the content of the MACROS
# run creates and runs the program in the terminal

CXX = c++
CFLAGS = -std=c++11
CXXSOURCE = cmdline.cpp main.cpp expr.cpp tests.cpp parse.cpp
OBJECTS = $(CXXSOURCE:.cpp=.o)
HEADERS = cmdline.hpp catch.h expr.h parse.hpp
LIBRARY = ../library/libmsdscript.a

run: msdscript
	./msdscript

msdscript: $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $(OBJECTS)

msdscript_from_Library: $(LIBRARY)
	$(CXX) $(CFLAGS) -o $@ main.cpp -L../library -lmsdscript

.PHONY: clean

clean:
	rm -f *.o *.out msdscript msdscript_from_Library prog_usingLib

test: msdscript
	./msdscript --test

interp: msdscript
	./msdscript --interp

printM:
	$(info $(HEADERS) $(CXXSOURCE))

doc: msdscript
	cd documentation && doxygen

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

$(LIBRARY): $(OBJECTS)
	ar -ruv $@ $(OBJECTS)
