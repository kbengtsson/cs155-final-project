varying vec3 N;
varying vec3 v;

void main()
{
    v = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));       
    N = normalize(gl_NormalMatrix * gl_Normal);
    // we want to render the shape using standard OpenGL position transforms.
    gl_Position = ftransform();
}
