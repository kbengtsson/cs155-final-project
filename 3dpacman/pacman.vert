varying vec3 n;
varying vec3 vpos;
varying vec4 pacman;
varying vec4 black;
varying vec4 shade1;
varying vec4 shade2;
varying vec4 shade3;

void main()
{
   	vpos = vec3(gl_ModelViewMatrix * gl_Vertex);       
   	n = normalize(gl_NormalMatrix * gl_Normal);
   	gl_Position = ftransform();  

   	black = vec4(0.0, 0.0, 0.0, 0.0);
   	shade1 = vec4(1.0, 0.95, 0.0, 1.0); // creates light shading
    shade2 = vec4(1.0, 0.93, 0.0, 1.0); // creates medium shading
    shade3 = vec4(1.0, 0.90, 0.0, 1.0); // creates dark shading
	pacman = vec4(1.0, 0.98, 0.0, 1.0); // creates pacman yellow
}