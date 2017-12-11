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

// shader file names
std::string vertexShader;
std::string fragmentShader;

SimpleShaderProgram *shader;
GLuint ProgramObject;

unsigned int cat_texture;

double rotateX = 0, rotateY = 0, zoom = 0;
int width = 100, height = 100;
double xScale = 10.0 / width, yScale = 10.0 / height;
double heightScale = 3.0;

void DrawWithShader(){

    shader->Bind();

    // shader->SetUniform("intensity", intensity);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    FastNoise::FastNoise myNoise; // Create a FastNoise object
    myNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type

    myNoise.SetFrequency(0.05);

    float heightMap[width][height]; // 2D heightmap to create terrain

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            heightMap[x][y] = myNoise.GetNoise(x,y);
        }
    }

    for (int row = 0; row < height - 1; ++row) {
        for (int col = 0; col < width - 1; ++col) {
            


            // glBegin(GL_TRIANGLES);
            // //glBegin(GL_TRIANGLE_STRIP);
            //     glVertex3f( col * xScale, row * yScale, z1 ); //vertex 1
            //     glVertex3f( col * xScale, (row + 1) * yScale, z2 ); //vertex 2
            //     glVertex3f( (col + 1) * xScale, row * yScale, z3 ); //vertex 3
            //     glVertex3f( (col + 1) * xScale, (row + 1) * yScale, z4 ); //vertex 4
            // glEnd();

            // Draw a triangle strip with 4 vertices

           
            float x1 = col * xScale, x2 = col * xScale, x3 = (col + 1) * xScale, x4 = (col + 1) * xScale;
            float y1 = row * yScale, y2 = (row + 1) * yScale, y3 = row * yScale, y4 = (row + 1) * yScale;
            float z1 = heightScale*heightMap[row][col], z2 = heightScale*heightMap[row + 1][col], 
                z3 = heightScale*heightMap[row][col + 1], z4 = heightScale*heightMap[row + 1][col + 1];

            SetNormalAndDrawTriangle(x1, x2, x3,
                                     y1, y2, y3,
                                     z1, z2, z3);

            // We put the vertices in this order to preserve the same direction of the vertices
            SetNormalAndDrawTriangle(x3, x2, x4,
                                    y3, y2, y4,
                                    z3, z2, z4);

            // SetNormalAndDrawTriangle(col * xScale, (col + 1) * xScale,(col + 1) * xScale,
            //                         (row + 1) * yScale, row * yScale, (row + 1) * yScale, 
            //                          z1, z2, z3);
            // std::cout << z1 << " " << z2 << " " << z3 << " " << z4 << " " << std::endl;
        }
    }

    shader->UnBind();
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

    glNormal3f(n1, n2, n3);
    glBegin(GL_TRIANGLES);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    glEnd();
}
// float * CreateNoise(int width, int height){
//     FastNoise myNoise; // Create a FastNoise object
//     myNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type

//     float heightMap[width][height]; // 2D heightmap to create terrain

//     for (int x = 0; x < width; x++)
//     {
//         for (int y = 0; y < height; y++)
//         {
//             heightMap[x][y] = myNoise.GetNoise(x,y);
//         }
//     }
// }


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

        default:
            break;
    }
    glFlush();
    glutPostRedisplay();
} 

void MouseCallback(int button, int state, int x, int y)
{
    glFlush();
    glutPostRedisplay();   
}

void Setup()
{
    shader = new SimpleShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);
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
    glutInitWindowSize(640, 480);
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

        // Enable lighting
    glEnable(GL_LIGHTING);  // turn on lighting (state variable)
    glEnable(GL_LIGHT0);    // enable the first light (# 0 ) 

    // set up the parameters of light 0:

    // position
    GLfloat light0_pos[]={10.0, 10.0, -10.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

    // diffuse color
    GLfloat diffuse0[]={.8, 0.2, 0.2};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);

    // specular color
    GLfloat specular0[]={1.0, 1., 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

    // ambient color
    GLfloat ambient0[]={0.8, 0.2, 0.2};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback);
    glutMouseFunc(MouseCallback);
    glutIdleFunc(DisplayCallback);

    glutMainLoop();
    return 0;
}


