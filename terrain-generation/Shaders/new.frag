varying vec3 N;
varying vec3 v;

uniform float intensity;
uniform float red;
uniform float green;
uniform float blue;

void main(void)
{
	// We color it black by default
	vec4 fragColor = vec4(0.0,0.0,0.0,0.0);
	float dot = dot(N,-v);
    if (dot < 0.5 && dot > -0.5){
    	// If point is close to orthogonal to viewing vector, color it
    	fragColor = vec4(red,green,blue,1.0);
    }

    gl_FragColor = fragColor * intensity;
}