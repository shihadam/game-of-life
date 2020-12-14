# Makefile for cs300 hw2
# Adam Shih 2020

CPP = g++
CPPFLAGS = -Wall -Werror -g
LFLAGS = -lncurses

conway: conway.cpp
	$(CPP) $(CPPFLAGS) -o conway conway.cpp $(LFLAGS)

clean:
	-rm -f conway
