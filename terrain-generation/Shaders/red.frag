// red.frag

/*
  this just colors everything red!
*/

varying float height;
varying vec4 exampleVarVec;
void main()
{
    // don't do anything interesting, just set the color
    // as this vector was set in the vertex shader.

    if (height < -2.0) {
    	gl_FragColor = vec4(0.0,0.0,0.85,1.0);
    }
    else if (height < 0.0) {
    	gl_FragColor = vec4(0.0,0.85, 0.0,1.0);
    }
    else if (height < 2.0) {
    	gl_FragColor = vec4(0.3,0.3, 0.3,1.0);
    }
    else {
    	gl_FragColor = exampleVarVec;
    }
}
