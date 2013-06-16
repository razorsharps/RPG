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
	initCheckPoints();

	// Get a handle for our "LightPosition" uniform
	glUseProgram(shaders[NORMAL]);
	LightID = glGetUniformLocation(shaders[NORMAL], "LightPosition_worldspace");
}

void Game::run() {
	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;

	int nbFrames = 0;

	Sound* s = new Sound();
	s->init();

	initText2D( "src/resources/Holstein.tga" );

	do{
		// Measure speed
		double currentTime = glfwGetTime();
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
		control->updateCar();
		control->updateCamera();

		// Use our shader
		glUseProgram(shaders[SKYBOX]);
		glm::mat4 TestProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 TestViewMatrix = control->getViewMatrix();
		glm::mat4 TestModelMatrix = glm::translate(glm::mat4(), control->getCarPosition());
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

		car->translateObject(control->getCarPosition());
		car->rotateObject(control->getCarDirection());

		tire1->setRotationSpeed(control->getRotationSpeed());
		tire1->setSteering(glm::vec3(control->getSteering(),0,0));
		tire2->setRotationSpeed(control->getRotationSpeed());
		tire2->setSteering(glm::vec3(control->getSteering(),0,0));
		tire3->setRotationSpeed(control->getRotationSpeed());
		tire4->setRotationSpeed(control->getRotationSpeed());

		renderer->renderObjects(ProjectionMatrix, ViewMatrix, handles[MATRIXID], handles[MODELMATRIXID], handles[VIEWMATRIXID]);
		
		if((glm::distance(car->getPosition(), box1->getPosition()) < 1.0f || glm::distance(car->getPosition(), box2->getPosition()) < 1.0f) && !playSound) {
			playSound = true;
			control->setCarSpeed(-1.0f);
			s->playSound();
		} else if (playSound && currentTime - lastTime > 0.9f) {
			playSound = false;
		}

		GLenum error = glGetError();
		if(error != GL_NO_ERROR) 
			std::cerr << "Opengl error " << error << ": " << (const char*) gluErrorString(error) << std::endl;

		char text[256];
		if(carMoved || control->getCarSpeed() != 0) {
			if(!carMoved) {
				carMoved = true;
				startTime = -glfwGetTime();
			}

			sprintf(text,"%.2f sec", startTime + glfwGetTime());
			printText2D(text, 10, 550, 25);
		}
		if(control->getCarSpeed() * 60 < 0.5) {
			sprintf(text,"%.1f km/h", 0.0f );
			printText2D(text, 10, 520, 25);
		} else {
			sprintf(text,"%.1f km/h", control->getCarSpeed() * 30 );
			printText2D(text, 10, 520, 25);
		}

		sprintf(text,"%.2f Last Lap Time", bestLap );
		printText2D(text, 10, 500, 14);

		checkLapTime();

		glfwSwapBuffers();
	} 
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

	glfwSetWindowTitle( "Peter Heinen RaceGame OpenGL" );

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
	Texture* tex = new Texture();
	tex->loadBMP_custom("src/resources/truck_color_cleantest.bmp");
	Texture* towerTex = new Texture();
	towerTex->loadBMP_custom("src/resources/bricks.bmp");
	
	Mesh* tireMesh = new Mesh();
	tireMesh->load("src/resources/cartirefix.obj");
	Mesh* carMesh = new Mesh();
	carMesh->load("src/resources/car.obj");
	Mesh* towerMesh = new Mesh();
	towerMesh->load("src/resources/cube.obj");
	 
	float height = 0.02f;

	car = new GameObject("Car", vec3(0, height, 0), 1);
	car->init(shaders[NORMAL]);
	car->setTexture(*tex);
	car->setMesh(carMesh);

	tire1 = new GameObject("RightFrontTire", vec3(-0.15f, height + 0.06f, 0.33f), 2.5f, true, vec3(0,0,0));
	tire1->init(shaders[NORMAL]);
	tire1->setTexture(*tex);
	tire1->setMesh(tireMesh);
	
	tire2 = new GameObject("LeftFrontTire", vec3(0.15f, height + 0.06f, 0.33f), 2.5f, true, vec3(0,0,pie));
	tire2->init(shaders[NORMAL]);
	tire2->setTexture(*tex);
	tire2->setMesh(tireMesh);

	tire3 = new GameObject("RightBackTire", vec3(-0.15f, height + 0.06f, -0.27f), 2.70f);
	tire3->init(shaders[NORMAL]);
	tire3->setTexture(*tex);
	tire3->setMesh(tireMesh);

	tire4 = new GameObject("LeftBackTire", vec3(0.15f, height + 0.06f, -0.27f), 2.70f, false, vec3(0,0,pie));
	tire4->init(shaders[NORMAL]);
	tire4->setTexture(*tex);
	tire4->setMesh(tireMesh);

	box1 = new GameObject("Box1", vec3(6, 0.2f, 4), 0.5f);
	box1->init(shaders[NORMAL]);
	box1->setTexture(*towerTex);
	box1->setMesh(towerMesh);

	box2 = new GameObject("Box2", vec3(6, 0.2f, -4), 0.5f);
	box2->init(shaders[NORMAL]);
	box2->setTexture(*towerTex);
	box2->setMesh(towerMesh);
	
	tire1->setParent(*car);
	tire2->setParent(*car);
	tire3->setParent(*car);
	tire4->setParent(*car);

	tire1->setTireBehaviour();
	tire2->setTireBehaviour();
	tire3->setTireBehaviour();
	tire4->setTireBehaviour();

	renderer = new Renderer();
	renderer->addObjects(*car);
	renderer->addObjects(*tire1);
	renderer->addObjects(*tire2);
	renderer->addObjects(*tire3);
	renderer->addObjects(*tire4);
	renderer->addObjects(*box1);
	renderer->addObjects(*box2);
}

void Game::initCheckPoints() {
	checkpoints[CHECK1] = glm::vec3(-13.5, 0, 6);
	checkpoints[CHECK2] = glm::vec3(-0.25, 0, 12.6);
	checkpoints[CHECK3] = glm::vec3(12.25, 0, 7.25);
	checkpoints[CHECK4] = glm::vec3(11.75, 0, -9.50);
	checkpoints[CHECK5] = glm::vec3(-13.25, 0, -7.65);
	checkpoints[CHECK6] = glm::vec3(3.75, 0, -0.95);
}

void Game::checkLapTime() {
	if(checkIndex > 5) {
		checkIndex = 0;
		bestLap = startTime + glfwGetTime();
		startTime = -glfwGetTime();
	} 

	if(glm::distance(car->getPosition(), checkpoints[checkIndex]) < 3.0f) {
		checkIndex += 1;
	}
}