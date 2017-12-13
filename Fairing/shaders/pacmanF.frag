varying vec3 n;
varying vec3 vpos;
varying vec4 pacman;
varying vec4 black;
varying vec4 shade1;
varying vec4 shade2;
varying vec4 shade3;

void main()
{	
	
	vec3 nVec = normalize(n);
	vec3 lVec = normalize(gl_LightSource[0].position.xyz - vpos);
	vec3 vVec = normalize(vpos);

	float NL = dot(nVec, lVec); 
	float NV = dot(nVec, vVec); 

    
	if (abs(NV) < 0.3)  {
        gl_FragColor = black; 
    } else if (abs(NL) < 0.15)  {
        gl_FragColor = shade3;
    } else if (abs(NL) < 0.35)  {
        gl_FragColor = shade2;
    } else if (abs(NL) < 0.55)  {
        gl_FragColor = shade1; 
    } else  {
    	gl_FragColor = pacman; 
    }
}
