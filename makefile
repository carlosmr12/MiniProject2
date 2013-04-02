CC := g++ -I/usr/include/libxml2
WARN := -Wall -Wextra -Wconversion

#   debug settings, uncomment when debugging
#   CCOPTS := $(WARN) -g

#   optimization setting, uncomment when done with debugging
	CCOPTS := $(WARN) -O3 -DNDEBUG

#   link executable when an .o file is newer
main : main.o stringHelper.o fileHelper.o
	$(CC) -o $@ $^ -lxml2

#   how to compile .c files
%.o : %.c
	$(CC) $(CCOPTS) -c -o $@ $<

#   remove object files and executable
clean:
	rm -rf *.o main

#	Execute phase2 script
phase2:
	./phase2

#   file dependencies: paste output of g++ -MM *.c here
#   if one file on the rhs is newer, use rule to update lhs
main.o : main.c stringHelper.h stringHelper.c fileHelper.h fileHelper.c constants.h
stringHelper.o : stringHelper.h 
fileHelper.o : fileHelper.h 
