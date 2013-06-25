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
#include "../headers/Time.h"
#include "../headers/Builder.h"

Game::Game() {
	initGLFW();
	initGLEW();
	setGlParameters();

	checkIndex = 0;

	carMoved = false;
	playSound = false;
	bestLap = 0.0f;

	double startTime = 0.0f;
	float pie = 3.14f;

	environment = new Environment();
	control = new Controls();
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

	// Get a handle for our "myTextureSampler" uniform
	TextureIDs  = glGetUniformLocation(shaders[NORMAL], "myTextureSampler");
	
	buildGameObjects();
	environment->initEnvironment();

	// Get a handle for our "LightPosition" uniform
	glUseProgram(shaders[NORMAL]);
	LightID = glGetUniformLocation(shaders[NORMAL], "LightPosition_worldspace");
}

void Game::run() {
	// For speed computation
	double lastTime = Time::getInstance().getTime();
	double lastFrameTime = lastTime;

	int nbFrames = 0;

	Sound* s = new Sound();
	s->init();

	initText2D( "src/resources/Holstein.tga" );

	do{
		// Measure speed
		double currentTime = Time::getInstance().getTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		control->accept(updateVisitor); 
		//control->update();
		//control->updateCamera();

		// Use our shader
		glUseProgram(shaders[SKYBOX]);
		glm::mat4 TestProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 TestViewMatrix = control->getViewMatrix();
		glm::mat4 TestModelMatrix = glm::translate(glm::mat4(), control->getPosition());
		glm::mat4 MVP = TestProjectionMatrix * TestViewMatrix * TestModelMatrix;

		glUniformMatrix4fv(handles[SKYBOXMATRIXID], 1, GL_FALSE, &MVP[0][0]);
		environment->drawEnvironment();

		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(handles[SKYBOXMATRIXID], 1, GL_FALSE, &MVP[0][0]);
		environment->drawPlane();

		glUseProgram(shaders[NORMAL]);

		ProjectionMatrix = control->getProjectionMatrix();
		ViewMatrix = control->getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(handles[MATRIXID], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(handles[MODELMATRIXID], 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(handles[VIEWMATRIXID], 1, GL_FALSE, &ViewMatrix[0][0]);
		
		glm::vec3 lightPos = glm::vec3(10,50,-20);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniform1i(TextureIDs, 0);

		halo->translateObject(control->getPosition());
		halo->rotateObject(control->getDirection());

/*		tire1->setRotationSpeed(control->getRotationSpeed());
		tire1->setSteering(glm::vec3(control->getSteering(),0,0));
		tire2->setRotationSpeed(control->getRotationSpeed());
		tire2->setSteering(glm::vec3(control->getSteering(),0,0));
		tire3->setRotationSpeed(control->getRotationSpeed());
		tire4->setRotationSpeed(control->getRotationSpeed());*/

		renderer->renderObjects(ProjectionMatrix, ViewMatrix, handles[MATRIXID], handles[MODELMATRIXID], handles[VIEWMATRIXID]);
		
		collision->update();
		/*if((glm::distance(halo->getPosition(), door1->getPosition()) < 1.0f)  && !playSound) {
/*		if((glm::distance(halo->getPosition(), door1->getPosition()) < 1.0f)  && !playSound) {
>>>>>>> af0375e67d4e64fd7dc4fab8fa92b511711ec6bc
			playSound = true;
			control->setSpeed(-1.0f);
			s->playSound();
		} else if (playSound && currentTime - lastTime > 0.9f) {
			playSound = false;
		}*/

		GLenum error = glGetError();
		if(error != GL_NO_ERROR) 
			std::cerr << "Opengl error " << error << ": " << (const char*) gluErrorString(error) << std::endl;

		char text[256];
		if(carMoved || control->getSpeed() != 0) {
			if(!carMoved) {
				carMoved = true;
				startTime = -Time::getInstance().getTime();
			}

			sprintf(text,"%.2f sec", startTime + Time::getInstance().getTime());
			printText2D(text, 10, 550, 25);
		}
		if(control->getSpeed() * 60 < 0.5) {
			sprintf(text,"%.1f km/h", 0.0f );
			printText2D(text, 10, 520, 25);
		} else {
			sprintf(text,"%.1f km/h", control->getSpeed() * 30 );
			printText2D(text, 10, 520, 25);
		}

		sprintf(text,"%.2f Last Lap Time", bestLap );
		printText2D(text, 10, 500, 14);

		glfwSwapBuffers();
	} 
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ));
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

	glfwSetWindowTitle( "Peter Heinen & Nick Velthorst RPG OpenGL" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void Game::setGlParameters() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
 
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(1);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Game::buildGameObjects() {
	Director dir;
	ObjectBuilder ob;
	dir.setBuilder(&ob);
	vector<GameObject*> go = dir.getGameObject(shaders[NORMAL]);
	
	renderer = new Renderer();

	halo = go.at(0);

	collision  = new Collision(halo);

	for(GameObject* g : go) {
		collision->addObjects(g);
		renderer->addObjects(g);
	}
}
