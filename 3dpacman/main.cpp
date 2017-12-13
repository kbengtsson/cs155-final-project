// on OSX, include GLUT this way:
#include <GLUT/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>


#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

#include "main.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

// shader file names
std::string vertexShader;
std::string fragmentShader;

SimpleShaderProgram *shader;

void KeyCallback(unsigned char key, int x, int y);

// variables used to parse obj files
struct Vertex3f   { float _x, _y, _z; } ;
struct Texture3f   { float _x, _y, _z; } ;
struct Normal3f    { float _x, _y, _z; };
struct Triangle3f { Vertex3f _a, _b, _c; Texture3f _ta, _tb, _tc; Normal3f _na, _nb, _nc; };

vector<Vertex3f> vertexLoc;   // v
vector<Texture3f> texture;     // vt
vector<Normal3f> normals;     // vn
vector<Triangle3f> triangles;   // f

// http://www.cplusplus.com/doc/tutorial/files/
int parseOBJ() {
    string line;
    ifstream myfile("pacman.obj");
    if (myfile.is_open()) {
        while (getline (myfile, line)) {
            // vertex locations
            if (line.substr(0,2) == "v ") {
                Vertex3f vLoc;
                istringstream rest(line.substr(2));
                rest >> vLoc._x;
                rest >> vLoc._y;
                rest >> vLoc._z;
                vertexLoc.push_back(vLoc);
            }
            // texture coordinates
            if (line.substr(0,2) == "vt") {
                Texture3f textCoord;
                istringstream rest(line.substr(2));
                rest >> textCoord._x;
                rest >> textCoord._y;
                rest >> textCoord._z;

                texture.push_back(textCoord);
            }
            // vertex normals from obj file
            if (line.substr(0,2) == "vn") {
                Normal3f normalCoord;
                istringstream rest(line.substr(2));
                rest >> normalCoord._x;
                rest >> normalCoord._y;
                rest >> normalCoord._z;

                normals.push_back(normalCoord);
            }
            // face data (triangles)
            if (line.substr(0,2) == "f ") {
                Triangle3f triFace;
                istringstream rest(line.substr(2));
                string a, b, c;
                rest >> a; // first chunk of the face
                rest >> b; // second chunk of the face
                rest >> c; // third chunk of the face

                int v, vt, vn;
                string parts[3] = {a, b, c};
                // loop that goes through three times to grab each data from the face
                for (int i = 0; i < 3; ++i) {
                    if (sscanf(parts[i].c_str(), "%i/%i/%i", &v, &vt, &vn) >= 2) {
                        if (i == 0) { // first chunk of the face, v1/t1 or v1/t1/n1
                            triFace._a = vertexLoc[v - 1];
                            triFace._ta = texture[vt - 1];
                            triFace._na = normals[vn - 1];
                        }
                        if (i == 1) { // second chunk of the face, v2/t2 or v2/t2/n2
                            triFace._b = vertexLoc[v - 1];
                            triFace._tb = texture[vt - 1];
                            triFace._nb = normals[vn - 1];

                        }
                        if (i == 2) { // third chunk of the face, v3/t3 or v3/t3/n3
                            triFace._c = vertexLoc[v - 1];
                            triFace._tc = texture[vt - 1];
                            triFace._nc = normals[vn - 1];
                        }
                    }
                    else if (sscanf(parts[i].c_str(), "%i//%i", &v, &vn) >= 2) {
                        if (i == 0) { // first chunk of the face, v1//n1
                            triFace._a = vertexLoc[v - 1];
                            triFace._na = normals[vn - 1];
                        }
                        if (i == 1) { // second chunk of the face, v2//n2
                            triFace._b = vertexLoc[v - 1];
                            triFace._nb = normals[vn - 1];

                        }
                        if (i == 2) { // third chunk of the face, v3//n3
                            triFace._c = vertexLoc[v - 1];
                            triFace._nc = normals[vn - 1];
                        }
                    }
                    else if (sscanf(parts[i].c_str(), "%i", &v) == 1) {
                        if (i == 0) { // first chunk of the face, v1
                            triFace._a = vertexLoc[v - 1];
                        }
                        if (i == 1) { // second chunk of the face, v2
                            triFace._b = vertexLoc[v - 1];
                        }
                        if (i == 2) { // third chunk of the face, v3
                            triFace._c = vertexLoc[v - 1];
                        }
                    }
                }
            triangles.push_back(triFace);
            }
        }
    }
    myfile.close();
    return 0;
}

void DrawWithShader(){
    shader->Bind();
    parseOBJ();
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangles.size(); ++i){
        // retrieve stored geometry and colors:
        float x1, y1, x2, y2, x3, y3, z1, z2, z3;
        float tx1,ty1,tz1,tx2,ty2,tz2,tx3,ty3,tz3;
        float nx1,ny1,nz1,nx2,ny2,nz2,nx3,ny3,nz3;

        // vertices of the triangle
        x1 = triangles[i]._a._x;   y1 = triangles[i]._a._y;     z1 = triangles[i]._a._z;
        x2 = triangles[i]._b._x;   y2 = triangles[i]._b._y;     z2 = triangles[i]._b._z;
        x3 = triangles[i]._c._x;   y3 = triangles[i]._c._y;     z3 = triangles[i]._c._z;

        // textures
        tx1 = triangles[i]._ta._x;   ty1 = triangles[i]._ta._y;     tz1 = triangles[i]._ta._z;
        tx2 = triangles[i]._tb._x;   ty2 = triangles[i]._tb._y;     tz2 = triangles[i]._tb._z;
        tx3 = triangles[i]._tc._x;   ty3 = triangles[i]._tc._y;     tz3 = triangles[i]._tc._z;

        // normals taken from the obj file
        nx1 = triangles[i]._na._x;   ny1 = triangles[i]._na._y;     nz1 = triangles[i]._na._z;
        nx2 = triangles[i]._nb._x;   ny2 = triangles[i]._nb._y;     nz2 = triangles[i]._nb._z;
        nx3 = triangles[i]._nc._x;   ny3 = triangles[i]._nc._y;     nz3 = triangles[i]._nc._z;

        glNormal3f(nx1, ny1, nz1);
        glTexCoord3f(tx1, ty1, tz1);
        glVertex3f(x1, y1, z1);

        glNormal3f(nx2, ny2, nz2);
        glTexCoord3f(tx2, ty2, tz2);
        glVertex3f(x2, y2, z2);

        glNormal3f(nx3, ny3, nz3);
        glTexCoord3f(tx3, ty3, tz3);
        glVertex3f(x3, y3, z3);
    }
    glEnd();
    // glutSolidTeapot(1.0);
    shader->UnBind();
}

float anglex = 0.0;
float angley = 0.0;
float anglez = 0.0;
float xaxis = 0.0;
float yaxis = 0.0;
void DisplayCallback(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glTranslatef(xaxis, yaxis, -10.0);   // everything we draw will be transformed down the Z axis
        glRotatef(anglex, 1.0, 0.0, 0.0);
        glRotatef(angley, 0.0, 1.0, 0.0);
        glRotatef(anglez, 0.0, 0.0, 1.0);
        DrawWithShader();                         // execute the code in my draw function
    glPopMatrix();

    glutSwapBuffers();

    glFlush();
}

float fov = 50.0;
double aspectRatio = 0.0;
void ReshapeCallback(int w, int h){
    glViewport(0, 0, w, h);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    aspectRatio = double(w)/double(h);

    //!!
    gluPerspective(fov, aspectRatio, 1.5, 20.0);
    // glFrustum(-1.5f, 1.5f, -1.5f, 1.5f, 1.f, 5.f);    // perspective camera
    // glOrtho(-1.5f, 1.5f, -1.5f, 1.5f, 1.f, 5.f);   // orthographic camera

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Setup()
{
    shader = new SimpleShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


int main(int argc, char** argv){

    // Check for the right number of arguments!
    if(!(argc == 3)){
        printf("usage: ./pacman <vertex shader> <fragment shader> \n");
        return 0;
    }

    // get the kernel file names from the command line
    vertexShader   = std::string(argv[1]);
    fragmentShader = std::string(argv[2]);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(640, 480);
    glutCreateWindow("CS155 Final Project");

    //
    // Initialize GLEW
    //
    #if !defined(__APPLE__) && !defined(__linux__)
    glewInit();
    if(!GLEW_VERSION_2_0) {
        printf("Your graphics card or graphics driver does\n"
               "\tnot support OpenGL 2.0, trying ARB extensions\n");

        if(!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
            printf("ARB extensions don't work either.\n");
            printf("\tYou can try updating your graphics drivers.\n"
                   "\tIf that does not work, you will have to find\n");
            printf("\ta machine with a newer graphics card.\n");
            exit(1);
        }
    }
    #endif

    Setup();

    // Enable lighting
    glEnable(GL_LIGHTING);  // turn on lighting (state variable)
    glEnable(GL_LIGHT0);    // enable the first light (# 0 ) 

    // set up the parameters of light 0:

    // position
    GLfloat light0_pos[]={10.0, 20.0, 5.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);


    // ambient color
    GLfloat ambient0[]={.6, 0.5, 1.0};
    GLfloat ambient1[]={0.25, 0.25, 0.25};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);

    // diffuse color
    GLfloat diffuse0[]={1.0, 1.0, 1.0};
    GLfloat diffuse1[]={0.75, 0.75, 0.75};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);

    // specular color
    GLfloat specular0[]={1.0, 1.0, 0.82};
    GLfloat specular1[]={1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular0);

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback);
    glutIdleFunc(DisplayCallback);

    glutMainLoop();
    return 0;
}



void KeyCallback(unsigned char key, int x, int y)
{
    switch(key) {
    case 'q': // quit
      exit(0);
    case 'i': // zoom in
        fov -= 0.9;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
            gluPerspective(fov, aspectRatio, 1.5, 20.0);
        break;
    case 'o': // zoom out
        fov += 0.9;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
            gluPerspective(fov, aspectRatio, 1.5, 20.0);
        break;
    case 'j': // rotate towards the viewer
        anglex += 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(anglex, 1.0, 0.0, 0.0);
        break;
    case 'k': // spin counterclockwise
        angley += 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(angley, 0.0, 1.0, 0.0);
        break;
    case 'l': // counterclockwise rotation
        anglez += 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(anglez, 0.0, 0.0, 1.0);
        break;
    case 'a': // rotate away from the viewer
        anglex -= 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(anglex, 1.0, 0.0, 0.0);
        break;
    case 's': // spin clockwise
        angley -= 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(angley, 0.0, 1.0, 0.0);
        break;
    case 'd': // clockwise rotation
        anglez -= 10;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glRotatef(anglez, 0.0, 0.0, 1.0);
        break;
    case 'n': // move up
        yaxis += 0.5;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glTranslatef(xaxis, yaxis, 0.0);
        break;
    case 'm': // move down
        yaxis -= 0.5;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glTranslatef(xaxis, yaxis, 0.0);
        break;
    case 'z': // move to the left
        xaxis -= 0.5;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glTranslatef(xaxis, yaxis, 0.0);
        break;
    case 'x': // move to the right
        xaxis += 0.5;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
            glTranslatef(xaxis, yaxis, 0.0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
