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

	// calculating curvature to help with shading/suggestive contours
	// adapted from http://madebyevan.com/shaders/curvature/
	vec3 dx = dFdx(nVec);
	vec3 dy = dFdy(nVec);
	vec3 xneg = nVec - dx;
	vec3 xpos = nVec + dx;
	vec3 yneg = nVec - dy;
	vec3 ypos = nVec + dy;
	float depth = length(vpos);
	float curvature = (cross(xneg, xpos).y - cross(yneg, ypos).x) * 4.0 / depth;
	vec3 rVec = normalize(-reflect(lVec, nVec)); // dot product of L vector and N vector
	float rFloat = dot(rVec, vec3(1.0)); // converting the R vector to a float
	float shading = dot(curvature,rFloat);
	

	// calculating the latitudinal lines
	// adapted from https://stackoverflow.com/questions/30891763/how-to-create-latitudinal-horizontal-contour-lines-in-glsl
	float width = 25.0;
	float distance = 10.0;
	vec3 stripe_thickness  = fract(line * width);
    vec3 stripe_width = fwidth(line * distance);
    vec3 stripes = smoothstep(stripe_width * 1.0, stripe_width * 2.0, stripe_thickness);


    // actual shading of the object by setting the colors
	if (abs(NV) < 0.3)  {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0); // creates black contour lines
    }
    else  {
    	gl_FragColor = vec4(1.0, 0.93, 0.0, 1.0); // creates pacman yellow
    }
}
