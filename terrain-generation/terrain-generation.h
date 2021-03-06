// main.h
#ifndef __TERRAIN_GENERATION_H__
#define __TERRAIN_GENERATION_H__

/*
  This file is used to pull in all the requirements
  for running a program with the GLEW library for
  loadng OpenGL extensions.

  Depending on how GLEW gets set up for your system,
  you may need to edit this file.
*/

/* Include-order dependency!
*
* GLEW must be included before the standard GL.h header.
*/
#ifdef __APPLE__
#define GLEW_VERSION_2_0 1
#elif defined(__linux__)
#define GLEW_VERSION_2_0 1
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#else
#define GLEW_STATIC 1
#include "GL/glew.h"
#endif

#include <string.h>
#include <stdexcept>
#include <cmath>

//
// Forward declarations?

void SetNormalAndDrawTriangle(float x1, float x2, float x3, 
                              float y1, float y2, float y3, 
                              float z1, float z2, float z3);

void RandomizeColors();

void DrawPacman();
//
// Headers.
//
#include "SimpleShaderProgram.h"
#include "findGLUT.h"

#endif // __MAIN_H__
