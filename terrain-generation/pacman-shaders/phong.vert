varying vec3 n;
varying vec3 vpos;
varying vec4 black;

void main()
{
   	vpos = vec3(gl_ModelViewMatrix * gl_Vertex);       
   	n = normalize(gl_NormalMatrix * gl_Normal);
   	gl_Position = ftransform();  

   	black = vec4(0.0, 0.0, 0.0, 0.0);
}
     