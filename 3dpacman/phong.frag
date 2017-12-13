// phong.frag

/*
  phong shading!
*/

// I = MaIa + (L*N)MdId + (R*V)^shine MsIs
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml

varying vec3 n;
varying vec3 vpos;
varying vec4 black;
varying vec4 shade1;
varying vec4 shade2;
varying vec4 shade3;

void main()
{
	vec4 Ia, Id, Is;
	vec4 ambient, diffuse, specular;
	
	// ambient color
	vec4 Ma = gl_FrontMaterial.ambient;
	ambient = Ma * gl_LightSource[0].ambient;
	
	// diffuse color
	vec4 Md = gl_FrontMaterial.diffuse;
	Id = Md * gl_LightSource[0].diffuse;
	// diffuse constant
	vec3 nVec = normalize(n);
	vec3 lVec = normalize(gl_LightSource[0].position.xyz - vpos);
	float NL = dot(nVec, lVec);
	// diffuse
	diffuse = clamp(Id * NL, 0.0, 1.0);

	// specular color
	vec4 Ms = gl_FrontMaterial.specular;
	Is = Ms * gl_LightSource[0].specular;
	// specular constant
	float shine = 100.0;
	vec3 rVec = normalize(-reflect(lVec, nVec));
	vec3 vVec = normalize(vpos);
	float RV = dot(rVec, vVec);
	//specular
	specular = clamp(Is * pow(RV, shine), 0.0, 1.0);

	// weights for each component. set to vec4(0.0) to turn off the component, set to vec4(1.0) to turn it on
	vec4 Wa = vec4(1.0);
	vec4 Wd = vec4(1.0);
	vec4 Ws = vec4(1.0);
	//multiply each part by weights (useful in debugging)
	gl_FragColor = Wa*ambient + Wd*diffuse;  

	float NV = dot(nVec, vVec);
	if (abs(NV) < 0.3)  {
        gl_FragColor = black; // creates black contour lines
    }
}
      