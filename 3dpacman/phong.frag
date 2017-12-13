varying vec3 n;
varying vec3 vpos;
varying vec4 black;

void main()
{
	vec4 Ia, Id, Is;
	vec4 ambient, diffuse;
	vec3 vVec = normalize(vpos);
	
	// ambient color
	vec4 Ma = gl_FrontMaterial.ambient;
	ambient = Ma * gl_LightSource[2].ambient;
	
	// diffuse color
	vec4 Md = gl_FrontMaterial.diffuse;
	Id = Md * gl_LightSource[2].diffuse;
	// diffuse constant
	vec3 nVec = normalize(n);
	vec3 lVec = normalize(gl_LightSource[0].position.xyz - vpos);
	float NL = dot(nVec, lVec);
	// diffuse
	diffuse = clamp(Id * NL, 0.0, 1.0);

	// weights for each component. set to vec4(0.0) to turn off the component, set to vec4(1.0) to turn it on
	vec4 Wa = vec4(1.0);
	vec4 Wd = vec4(1.0);
	//multiply each part by weights (useful in debugging)
	gl_FragColor = Wa*ambient + Wd*diffuse;  

	float NV = dot(nVec, vVec);
	if (abs(NV) < 0.3)  {
        gl_FragColor = black; // creates black contour lines
    }
}
      