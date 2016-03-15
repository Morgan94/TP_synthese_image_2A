#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>


void init(void);
void initGLFW();
void initGLEW();
void renderFrame();
void make_ressources();
void initShader();
void* file_contents(GLchar *filename, GLint * length);

//GLuint normalBuffer;

GLuint vertexCount = 3;

float positions[]={0,0,0, 0,1,0, 1,0,0, 1,1,0, 1,-1,0, 0,-1,0, -1,1,0, -1,0,0, -1,-1,0 };

//float normals[]={0,0,0,0,0,0,0,0,0};
unsigned int index[]={5,4,2, 2,3,1, 1,6,7, 7,8,5 };
GLuint indexCount = 12;
GLint positionIndex;
GLint normalIndex;
