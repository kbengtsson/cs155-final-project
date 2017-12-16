# cs155-final-project
3D Pac-man with random terrain generation and smoothing.

# Contributions
Annalise worked on building a 2D and 3D Pac-Man using OpenGL. She also worked on creating the Pac-Man obj file using Blender, as well as creating the GLSL toon shader and phong shader for 3D Pac-Man.

Kevin worked on creating the randomly generated terrain using Perlin noise and FastNoise, coloring it based on height, creating the shaders for it, and putting Pac-Man into the scene to traverse over the terrain.

# How to run the interactive Pac-Man program:

1. In terminal, navigate to the terrain-generation folder
2. Run make
3. Run the command
```
./terrain-generation terrain-shaders/phong.vert terrain-shaders/phong.frag
```
Each step should only take a couple of seconds! If not, something has likely gone wrong.

# Using the program:

You can navigate using the WASD keys to rotate the camera, TFGH to move Pacman around on the X-Z plane, Z and X to zoom in and out respectively and R to randomize the generated terrain.

# Final image:
![Pacman](https://raw.githubusercontent.com/kbengtsson/cs155-final-project/master/pacman-final.jpg)
