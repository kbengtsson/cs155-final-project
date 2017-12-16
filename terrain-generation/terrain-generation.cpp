#include "terrain-generation.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

#include <vector>
#include <cstdio>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <iostream>
#include "FastNoise.h"
using namespace std;

#define width 500
#define height 500 
#define radius 0.2
// shader file names
std::string vertexShader;
std::string fragmentShader;

SimpleShaderProgram *shader;
SimpleShaderProgram *shader2;
GLuint ProgramObject;

unsigned int cat_texture;

int seed, moveSide = 0, moveUp = 0, rotatePacman = 0;

double rotateX = 0, rotateY = 0, zoom = 0;
double xScale = 1.0 / 50.0, zScale = 1.0 / 50.0;
double xOffset = -(width*xScale) / 2.0, zOffset = -(height*zScale)/2.0;
double heightScale = 3.0;

float heightMap[width][height]; // 2D heightmap to create terrain

double intensity = 1.0;
double red = 1.0, green = 1.0, blue = 1.0;

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
    ifstream myfile("pman.obj");
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

void DrawPacman(){
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
}

void DrawWithShader(){
    shader->Bind();

    shader->SetUniform("intensity", intensity);
    shader->SetUniform("red", red);
    shader->SetUniform("green", green);
    shader->SetUniform("blue", blue);

    // shader->SetUniform("intensity", intensity);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // We create three different instances of Perlin noise
    // Each one has a different frequency for more realistic terrain
    FastNoise::FastNoise noise1; 
    noise1.SetNoiseType(FastNoise::Perlin); 
    noise1.SetFrequency(0.005);
    noise1.SetSeed(seed);

    FastNoise::FastNoise noise2;
    noise2.SetNoiseType(FastNoise::Perlin);
    noise2.SetFrequency(0.01);
    noise2.SetSeed(seed);

    FastNoise::FastNoise noise3;
    noise3.SetNoiseType(FastNoise::Perlin);
    noise3.SetFrequency(0.02);
    noise3.SetSeed(seed);

    // Populate height map with Noise values
    for (int x = 0; x < width; x++)
    {
        for (int z = 0; z < height; z++)
        {
            // We add our noise together, scaled appropriately
            heightMap[x][z] = noise1.GetNoise(x,z) + 
                              noise2.GetNoise(x,z)*0.5 + 
                              noise3.GetNoise(x,z)*0.25;
        }
    }

    // Loop through the desired width and height and draw a grid with y-values modulated by height map
    for (int row = 0; row < height - 1; ++row) {
        for (int col = 0; col < width - 1; ++col) {
           
            float x1 = col * xScale + xOffset, x2 = col * xScale + xOffset, 
                  x3 = (col + 1) * xScale + xOffset, x4 = (col + 1) * xScale + xOffset;

            float y1 = heightScale*heightMap[row][col], y2 = heightScale*heightMap[row + 1][col], 
                y3 = heightScale*heightMap[row][col + 1], y4 = heightScale*heightMap[row + 1][col + 1];

            float z1 = row * zScale + zOffset, z2 = (row + 1) * zScale + zOffset, 
                  z3 = row * zScale + zOffset, z4 = (row + 1) * zScale + zOffset;
            
            SetNormalAndDrawTriangle(x1, x2, x3,
                                     y1, y2, y3,
                                     z1, z2, z3);

            SetNormalAndDrawTriangle(x3, x2, x4,
                                     y3, y2, y4,
                                     z3, z2, z4);

        }
    }
    shader->UnBind();

    // Here we draw Pac-Man
    shader2->Bind();
    glPushMatrix();
        glTranslatef(moveSide*xScale, heightMap[height/2 + moveUp][width/2 + moveSide]*heightScale + radius, moveUp*zScale);
        glRotatef(rotatePacman, 0, 1, 0);
        glScalef(0.2,0.2,0.2);
        DrawPacman();
    glPopMatrix();

    shader2->UnBind();
}

void SetNormalAndDrawTriangle(float x1, float x2, float x3, 
                              float y1, float y2, float y3, 
                              float z1, float z2, float z3) {
    float u1, u2, u3;
    float v1, v2, v3;

    u1 = x2 - x1; u2 = y2 - y1; u3 = z2 - z1;
    v1 = x3 - x2; v2 = y3 - y2; v3 = z3 - z2; 

    float n1 = u2*v3 - u3*v2;
    float n2 = u3*v1 - u1*v3;
    float n3 = u1*v2 - u2*v1;

    float mag = sqrt(n1*n1 + n2*n2 + n3*n3); 

    n1 /= mag; n2 /= mag; n3 /= mag;

    // We set one normal per triangle face
    glNormal3f(n1, n2, n3);
    glBegin(GL_TRIANGLES);
        float avgHeight = (y1 + y2 + y3) / 3.0;

        // Triangle color depends on height of terrain
        if (avgHeight < -1.0) {
            glColor3f(0.0,.467,0.745);
        }
        else if (avgHeight < -0.5) {
            glColor3f(0.0,104.0/255.0, 10.0/255.0);
        }
        else if (avgHeight < 1.5) {
            glColor3f(51.0/255.0,25.0/255.0, 0.0);
        }
        else {
            glColor3f(1.0,1.0,1.0);
        }

        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    glEnd();
}

void DisplayCallback(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -10.f);

    glTranslatef(0, 0, -5 + zoom);
    glRotatef(rotateX, 0, 1, 0);
    glRotatef(rotateY, 1, 0, 0);

    DrawWithShader();
    glutSwapBuffers();
}

void ReshapeCallback(int w, int h){
    glViewport(0, 0, w, h);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(30.0f, (float)w/(float)h, 0.1f, 100000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KeyCallback(unsigned char key, int x, int y)
{
    //glLoadIdentity();
    switch(key) {
        case 'q':
            exit(0);
            break;
        // Zoom In
        case 'z':
            zoom += 0.1;
            break;
        // Zoom Out
        case 'x':
            zoom -= 0.1;
            break;
        case 'w':
            rotateY += 5;
            break;
        case 'a':
            rotateX += 5;
            break;
        case 'd':
            rotateX -= 5;
            break;
        case 's':
            rotateY -= 5;
            break;
        case 'i':
            intensity = fmin(1.0, intensity + 0.1);
            break;
        case 'u':
            intensity = fmax(-1.0, intensity - 0.1);
            break;
        // Randomize terrain by changing the random seed
        case 'r':
            seed = rand();
            break;
        case 't':
            moveUp -= 5;
            rotatePacman = 90;
            break;
        case 'g':
            moveUp += 5;
            rotatePacman = -90;
            break;
        case 'f':
            moveSide -= 5;
            rotatePacman = 180;
            break;
        case 'h':
            moveSide += 5;
            rotatePacman = 0;
            break;
        default:
            break;
    }
    glFlush();
    glutPostRedisplay();
} 

void MouseCallback(int button, int state, int x, int y)
{
    switch(button){   
        case GLUT_LEFT_BUTTON:
            RandomizeColors();
            break;
        default:
            break;
    }
    glFlush();
    glutPostRedisplay();   
}

void RandomizeColors(){
    red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Setup()
{
    shader = new SimpleShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void SetupPacmanShader()
{
    shader2 = new SimpleShaderProgram();
    shader2->LoadVertexShader("pacman-shaders/phong.vert");
    shader2->LoadFragmentShader("pacman-shaders/phong.frag");
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void init(void)
{
    /* select clearing (background) color */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    // Initialize the random seed for Perlin Noise
    seed = rand();
    //CreateNoise(width, height);

}

int main(int argc, char** argv){

    // Check for the right number of arguments!
    if(!(argc == 3)){
        printf("usage: ./hw5 <vertex shader> <fragment shader> \n");
        return 0;
    }

    // get the kernel file names from the command line
    vertexShader   = std::string(argv[1]);
    fragmentShader = std::string(argv[2]);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("CS155 Assignment Final Project");
    init();

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
    SetupPacmanShader();

        // Enable lighting
    glEnable(GL_LIGHTING);  // turn on lighting (state variable)
    glEnable(GL_LIGHT0);    // enable the first light (# 0 ) 

    // set up the parameters of light 0:

    // position
    GLfloat light0_pos[]={10.0, 10.0, -10.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

    // diffuse color
    GLfloat diffuse0[]={.2, 0.2, 0.22};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);

    // specular color
    GLfloat specular0[]={1.0, 1., 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

    // ambient color
    GLfloat ambient0[]={0.2, 0.2, 0.22};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);

    GLfloat ambient2[]={1.0, 0.98, 0.0, 1.0};

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);

    GLfloat diffuse2[]={1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);


    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback);
    glutMouseFunc(MouseCallback);
    glutIdleFunc(DisplayCallback);

    glutMainLoop();
    return 0;
}


