all: terrain-generation

# ----------------------------------------------------------------------------------------

# the -Wno-deprecated flag is used here to silence warnings about old timey GLUT functions

terrain-generation:	terrain-generation.cpp
	g++ -Wno-deprecated  -framework OpenGL -framework GLUT -o terrain-generation terrain-generation.cpp

# ----------------------------------------------------------------------------------------

clean:
	rm -rf terrain-generation *~ .#*
