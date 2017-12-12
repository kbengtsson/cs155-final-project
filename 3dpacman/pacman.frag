varying vec3 n;
varying vec3 vpos;
varying vec3 line;

void main()
{	
	// normalizing all the vectors
	vec3 nVec = normalize(n);
	vec3 lVec = normalize(gl_LightSource[0].position.xyz - vpos);
	vec3 vVec = normalize(vpos);

	float NL = dot(nVec, lVec); // dot product of N vector and L vector
	float NV = dot(nVec, vVec); // dot product of N vector and V vector

    // actual shading of the object by setting the colors
	if (abs(NV) < 0.3)  {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0); // creates black contour lines
    }
    else  {
    	gl_FragColor = vec4(1.0, 0.93, 0.0, 1.0); // creates pacman yellow
    }
}
