// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
 
// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>

#include "../headers/Game.h"
#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/objloader.h"
#include "../headers/vboindexer.h"
#include "../headers/functions.h"
#include "../headers/controls.h"
#include "../headers/GameObject.h"

enum {NORMAL = 0, SKYBOX};
GLuint shaders[2]; 

enum {MATRIXID = 0, VIEWMATRIXID, MODELMATRIXID, SKYBOXMATRIXID};
GLuint handles[4]; 

GLuint TextureIDs;
GLuint Textures;	

Game::Game() {
	initGLFW();
	initGLEW();
	setGlParameters();
}

Game::~Game() {

}

void Game::build() {
	// Create and compile our GLSL program from the shaders
	shaders[NORMAL]		= LoadShaders( "src/shaders/StandardShading.vertexshader", "src/shaders/StandardShading.fragmentshader" );
	shaders[SKYBOX]		= LoadShaders( "src/shaders/TransformVertexShader.vertexshader", "src/shaders/TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	handles[MATRIXID]			= glGetUniformLocation(shaders[NORMAL], "MVP");
	handles[VIEWMATRIXID]		= glGetUniformLocation(shaders[NORMAL], "V");
	handles[MODELMATRIXID]		= glGetUniformLocation(shaders[NORMAL], "M");
	
	handles[SKYBOXMATRIXID]		= glGetUniformLocation(shaders[SKYBOX], "MVP");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID	= glGetAttribLocation(shaders[NORMAL], "vertexPosition_modelspace");
	GLuint vertexUVID					= glGetAttribLocation(shaders[NORMAL], "vertexUV");
	GLuint vertexNormal_modelspaceID	= glGetAttribLocation(shaders[NORMAL], "vertexNormal_modelspace");

	// Load a car GameObject
	GameObject* car = new GameObject("Car", vec3(0,0,0));
	Texture* carTex = new Texture();
	carTex->loadBMP_custom("src/shaders/truck_color_cleantest.bmp");
	Mesh* carMesh = new Mesh();
	carMesh->load("src/shaders/car.obj");
	car->setTexture(carTex);
	car->setMesh(carMesh);

	// Get a handle for our "myTextureSampler" uniform
	TextureIDs  = glGetUniformLocation(shaders[NORMAL], "myTextureSampler");

	// Get a handle for our "LightPosition" uniform
	glUseProgram(shaders[NORMAL]);
	GLuint LightID = glGetUniformLocation(shaders[NORMAL], "LightPosition_worldspace");

	initEnvironment();
}

void Game::run() {
	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaders[NORMAL]);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(handles[MATRIXID], 1, GL_FALSE, &MVP[0][0]);
		
		// Use our shader
		glUseProgram(shaders[SKYBOX]);
		
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(handles[SKYBOXMATRIXID], 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Textures);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureIDs, 0);

		drawEnvironment();
	
		GLenum error = glGetError();
		if(error != GL_NO_ERROR) 
			std::cerr << "Opengl error " << error << ": " << (const char*) gluErrorString(error) << std::endl;

		// Swap buffers
		glfwSwapBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );
}

void Game::stop() {
	glfwTerminate();
}

void Game::initGLFW() {
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
	}
}

void Game::initGLEW() {
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetWindowTitle( "Peter Test OpenGL" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void Game::setGlParameters() {
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
 
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}