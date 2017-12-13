varying vec3 n;
varying vec3 vpos;
varying vec4 pacman;
varying vec4 black;
varying vec4 shade1;
varying vec4 shade2;
varying vec4 shade3;

void main()
{	
	// normalizing all the vectors
	vec3 nVec = normalize(n);
	vec3 lVec = normalize(gl_LightSource[0].position.xyz - vpos);
	vec3 vVec = normalize(vpos);

	float NL = dot(nVec, lVec); // dot product of N vector and L vector
	float NV = dot(nVec, vVec); // dot product of N vector and V vector

    // actual shading (with toon shading) of the object 
	if (abs(NV) < 0.3)  {
        gl_FragColor = black; // creates black contour lines
    } else if (abs(NL) < 0.15)  {
        gl_FragColor = shade3; // creates dark shading
    } else if (abs(NL) < 0.35)  {
        gl_FragColor = shade2; // creates medium shading
    } else if (abs(NL) < 0.55)  {
        gl_FragColor = shade1; // creates light shading
    } else  {
    	gl_FragColor = pacman; // creates pacman yellow
    }
}