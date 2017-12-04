// using OSX, so I include GLUT this way:
#include <GLUT/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// local
#include "util.h"

void KeyCallback(unsigned char key, int x, int y);

void drawTriangle() {
    glBegin(GL_TRIANGLES);
        glVertex3f(0,0,0);
        glVertex3f(0.3,0.75,0);
        glVertex3f(0,0.8,0);
    glEnd();

    glFlush();
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2 * 3.141592; 

    glBegin(GL_LINE_LOOP);
        for(i = 0; i <= lineAmount;i++) { 
            glVertex2f(
                x + (radius * cos(i * twicePi / lineAmount)), 
                y + (radius * sin(i * twicePi / lineAmount))
            );
        }
    glEnd();
    glutSwapBuffers();
}

double move = 0.0; 

void drawPacman(double radius)   // the filled one
{
    // Specify how much you want the mouth open
    double open = sin( move ) * 0.5 + 0.5; // range is from 0 - 1.0

    // where does the upper lip start at?
    double upperLip = PI/4.0f * open;
    // make the lower lip the opposite that
    double lowerLip = 2.0f*PI - upperLip;

    // translate and rotate pac man's position and orientation
    glBegin( GL_TRIANGLE_FAN );
    glVertex2f( 0.0f, 0.0f );   // center of packman

    float theta, smoothness = PI/25;  // smaller number = smoother packman
    // draw around packman starting at the lips - this is oriented to the right
    for( double theta = upperLip; theta <= lowerLip; theta += smoothness )
    {
     glVertex2f( cos( theta )*radius, sin( theta )*radius );
    }

    glFlush();
}

void Draw(){
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0, 0.93, 0.0, 1.0);
        drawPacman(1.0);
        glEnd();

}

void DisplayCallback(){

    // the arguments tell glClear which buffers to clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); // wipe the modelview matrix
    glLoadIdentity();

    Draw();                     // execute the code in my draw function
    glFlush();                  // tells OpenGL to send what we've drawn to GPU, display
}


void ReshapeCallback(int w, int h){
    glViewport(0, 0, w, h);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv){
    // Initialize GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CS155: Now with even more triangles!!!");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutIdleFunc(DisplayCallback);
    glutKeyboardFunc(KeyCallback);

    glutMainLoop();
    return 0;
}

void KeyCallback(unsigned char key, int x, int y)
{
    switch(key) {
    case 'q':
      exit(0);
    case 'i':
        move += 0.5;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        break;
    case 'o':
        move -= 0.5;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
