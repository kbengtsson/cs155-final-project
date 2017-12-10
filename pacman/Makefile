all: pacman

# ----------------------------------------------------------------------------------------

# the -Wno-deprecated flag is used here to silence warnings about old timey GLUT functions

pacman:	pacman.cpp
	g++ -Wno-deprecated  -framework OpenGL -framework GLUT -o pacman pacman.cpp

# ----------------------------------------------------------------------------------------

clean:
	rm -rf pacman *~ .#*