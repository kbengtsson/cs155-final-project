Here are all the resources I used:
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml  to figure out how to call glMaterialfv in my main.cpp file

I wanted to create the teapot but did not want to comment out all my code where I parse an obj file and just call "glSolidTeapot" for the creative shading portion. I found this https://github.com/McNopper/OpenGL/blob/master/Binaries/teapot.obj online and am using it because it also has a "vn" component in the obj file so that I can just find the normals from the obj file rather than calculating it from each face.

I also found an elephant obj file in the same repository as the teapot one. I currently have it so that when you run my code, the elephant is the image that it renders. You can access it in the meshes folder.

For my creative shading, I also found this article: http://users.csc.calpoly.edu/~zwood/teaching/csc572/final10/acdaw/ when I was trying to calculate the suggested contours. I don't think my calculations are correct, but I managed to get some sort of shading. I had a hard time trying to figuring out how to find the radial curvature, so I found this page http://madebyevan.com/shaders/curvature/ and applied it to my shader. I also added on the latitudinal shading from: https://stackoverflow.com/questions/30891763/how-to-create-latitudinal-horizontal-contour-lines-in-glsl and played around with the coordinates in my own shader. I also found something on toon shading and realized that if I set limits around the dot product of the N vertex and the L vertex then I could create multiple layers or levels of toon shading. For my creative shading, however, I just added in one level, otherwise the entire image would have too many colors and it didn't highlight the contours as much, so I did not include it. It would also have been just overwhelming to look at with all the colors.

Total hours on phong shading: 2.5 hours
Total hours on creative shading: 5-6 hours
Total hours meeting with the professor: 45 minutes - 1 hour
Total hours on this assignment: approx. 10 hours