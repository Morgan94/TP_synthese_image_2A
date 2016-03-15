#include <iostream>
#include <fstream>
#include "myOwnOpenGLProg.h"

/*!todo: You must code an openGL application from scratch here;
 * You can rely on openGL, glfw3, glm BUT NOT ON the labs api: lightGLAPI.h
 * Here are the details of your job:
 * Lab 0: Create a triangle
 * Lab 1: Create some more complex primitive (e.g parametric) and handle
 * occlusions
 * Lab 2: Add some textures
 * Lab 3: Add some nice shading effects
 * 
 * */



// include -> BFC, ZBuffer, Tore, Unifrom mat4, call back ? 

//-------------------- Inits
const int width = 1024, height = 800;

 GLuint positionBuffer, indexBuffer;
 GLuint vao;
 GLuint programId;

void *file_contents(const char *filename, GLint *length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}

void init(void)
{

    initGLFW();
    initGLEW();
    initShader();
    make_ressources();
}

void initShader(){

    //---- CPU side for shader program
      GLint vLength, fLength;

      const GLchar *vSource = (const GLchar*)file_contents("myOwnGLSLProg.v.glsl", &vLength);
      const GLchar *fSource = (const GLchar*)file_contents("myOwnGLSLProg.f.glsl", &fLength);
	  
      //---- GPU side (Load and compile)
      GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vShader, 1, (const GLchar**)&vSource, &vLength);
      glCompileShader(vShader);
      
      GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fShader, 1, (const GLchar**)&fSource, &fLength);
      glCompileShader(fShader);

      //---- gpu side (link)
      programId = glCreateProgram();
      glAttachShader(programId,vShader);
      glAttachShader(programId,fShader);
      glLinkProgram(programId);
}

void make_ressources(){


	// VBO
	positionBuffer = 0;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount* 3 * sizeof(float), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	// IBO
	indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    // VAO
	vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    //--------- Encapsulation of VBOs
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLuint positionIndex = glGetAttribLocation(programId,"position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Yet another VBO
    /*glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    normalIndex = glGetAttribLocation(programId, "normal");
    glEnableVertexAttribArray(normalIndex);
    glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

    //--------- Encapsulation of IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
    //--------- Deactivation (clean OpenGl state!!)
    glBindVertexArray(0);



}

void initGLFW() {
  if (!glfwInit()) exit(1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a window of given size
  const int window_width = 1024, window_height = 800;
  GLFWwindow* window =glfwCreateWindow(window_width, window_height,
        "Minimal OpenGL application with GLFW", NULL, NULL);
  if (!window) {
      std::cerr<<"Could not open a window"<<std::endl;
      exit(1);
  }
  // Make the context of the specified window current
  glfwMakeContextCurrent(window);
}


void initGLEW(){
    /* GLEW Initialization */
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK) {
        std::cerr<<"ERROR: "<< glewGetErrorString(GlewInitResult)<<std::endl;
        exit(1);
    }
}


void renderFrame() {

    //--------- clear the color-buffer and the depth-buffer, set some states
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    
    //--------- bind shader program [and textures] (see later...)
    glUseProgram(programId);
    //--------- Activate the VAO and call the drawing routine
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT,NULL);
    //--------- Clean state again
    glBindVertexArray(0);
    
    
    glUseProgram(0);
    //--------- Swap back and front buffers
    GLFWwindow* window = glfwGetCurrentContext();
    glfwSwapBuffers(window);
    //--------- empty the queue of events or the app will freeze
    glfwPollEvents();

}

void shutDown(int return_code)/*{{{*/
{
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    exit(return_code);
}


int main(void)
{
    init();

    GLFWwindow* window = glfwGetCurrentContext();
    while(!glfwWindowShouldClose(window))
    {
        renderFrame();
    }

    shutDown(0);

}
