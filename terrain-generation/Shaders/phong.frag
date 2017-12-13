
varying vec3 N;
varying vec3 v;

void main(void)
{
    vec4 I_amb = gl_LightSource[0].ambient;
    vec4 I_diff = gl_LightSource[0].diffuse;
    vec4 I_spec = gl_LightSource[0].specular;

    // Hard code some colors for the material
    vec4 M_amb = gl_Color;
    vec4 M_diff = gl_Color;
    vec4 M_spec = vec4(0.5,0.5,0.5,1.0);

    vec3 L = normalize(gl_LightSource[0].position.xyz - v);

    vec3 R = normalize(-reflect(L, N));

    float shine = 1.0;

    // Compute the phong shading
    vec4 fragColor = M_amb*I_amb + M_diff*I_diff*clamp(dot(L, N), 0.0, 1.0) + M_spec*I_spec*pow(clamp(dot(R, normalize(-v)), 0.0, 1.0), shine);

    gl_FragColor = fragColor;
}