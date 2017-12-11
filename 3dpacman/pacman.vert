// annalise.vert

/*
  This simple GLSL vertex shader takes in the color from
  the fragment shader using a "creative shading" and computes the position
  of the vertex (to be interpolated automatically, and used
  in the fragment shader)
*/

varying vec3 n;
varying vec3 vpos;
varying vec3 line;

void main()
{
  	line = gl_Vertex.xyz;
   	vpos = vec3(gl_ModelViewMatrix * gl_Vertex);       
   	n = normalize(gl_NormalMatrix * gl_Normal);
   	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
}