# Maze Solver Team
# Git Repo: https://github.com/andujarnick/SoftwareToolsMazeSolver
# Project Makefile

#Mark targets as non-file
.PHONY: all build archive clean

#default make to make the executable and archive
all: build archive

#calls for the executable
build: maze_solver

#builds the maze_solver executable
maze_solver: map.cc
	$(CXX) -Wall map.cc

#calls for the archive 
archive: archive.tar.gz

#builds the archive
archive.tar.gz: map.cc
	tar czf archive.tar.gz $^

#cleans the executable
clean:
	-rm -f maze_solver
