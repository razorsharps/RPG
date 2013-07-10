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
	win = false;
	bestLap = 0.0f;
	double startTime = 0.0f;
	float pie = 3.1415f;

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
	LightID1 = glGetUniformLocation(shaders[NORMAL], "LightPosition_worldspace");
	LightID2 = glGetUniformLocation(shaders[NORMAL], "LightPosition_worldspaceLocal");
}

void Game::run() {
	// For speed computation
	double lastTime = Time::getInstance().getTime();
	double lastFrameTime = lastTime;

	int nbFrames = 0;

	initText2D( "src/resources/Holstein.tga" );
	GameObject * go = new GameObject("WINNING STATUE", glm::vec3(-6.0f, 0.0f, 6.0f ), glm::vec3(0.04f), false, glm::vec3(180,0,0));
	Mesh * mesh = new Mesh("src/resources/Halo.obj");
	Texture * texture = new Texture("src/resources/bricks.bmp");
	go->setMesh(mesh);
	go->setTexture(texture);	
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

		if(Inventory::getKey("Key1") && Inventory::getKey("Key2") && Inventory::getKey("Key3") && Inventory::getKey("Key4") && !win){
			renderer->addObjects(go);
			win = true;
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
		//environment->drawEnvironment();

		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(handles[SKYBOXMATRIXID], 1, GL_FALSE, &MVP[0][0]);
		//environment->drawPlane();

		glUseProgram(shaders[NORMAL]);

		ProjectionMatrix = control->getProjectionMatrix();
		ViewMatrix = control->getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(handles[MATRIXID], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(handles[MODELMATRIXID], 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(handles[VIEWMATRIXID], 1, GL_FALSE, &ViewMatrix[0][0]);
		
		glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 10.0f);
		glUniform3f(LightID1, lightPos.x, lightPos.y, lightPos.z);

		glm::vec3 lightPos2 = glm::vec3((-10.0f, 10.0f, -10.0f));
		glUniform3f(LightID2, lightPos2.x, lightPos2.y, lightPos2.z);
		glUniform1i(TextureIDs, 0);

		halo->translateObject(control->getPosition());
		halo->rotateObject(control->getDirection());

		std::vector<GameObject*>::iterator it;

		for(it = renderer->gameObjects.begin()+1; it != renderer->gameObjects.end(); ++it)
		{
			Astroid * anAstroid = dynamic_cast<Astroid*>(*it);
			if( anAstroid != 0 ) {
				if((*it)->name != "HaloCharacter") {
					glm::mat4 RotationMatrix		= eulerAngleYXZ((anAstroid)->orientation.x, (anAstroid)->orientation.y, (anAstroid)->orientation.z);
					glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
					glm::vec3 realforward(forward);
			//		anAstroid->position -= realforward * deltaTime * anAstroid->speed * 1.0f;
				}
			}
		}
		
		renderer->renderObjects(ProjectionMatrix, ViewMatrix, handles[MATRIXID], handles[MODELMATRIXID], handles[VIEWMATRIXID]);
		std::vector<GameObject*> meuk;
		octree->gatherObjects(meuk);
		std::vector<GameObject*>::iterator iter;
		for(iter=meuk.begin(); iter != meuk.end(); ++iter) {
			octree->add(*iter);
		}

		octree->detectCollisions();
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
		sprintf(text,"%d Astroids left!", renderer->gameObjects.size() - 1); //because halo
		printText2D(text, 10,580, 16);

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
	int min = -50;
	int max = 100;
	octree = new Octree(glm::vec3(0), 55.0f, 10);
	Director dir;
	ObjectBuilder ob;
	dir.setBuilder(&ob);
	vector<GameObject*> go = dir.getGameObject(shaders[NORMAL]);
	
	renderer = new Renderer();

	Mesh * mesh = new Mesh("src/resources/ball.obj");
	Texture * texture = new Texture("src/resources/land.bmp");
	for ( int i = 0; i < 1000; ++i ) {		
		float x       = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float y		  = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float z		  = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float scale   = 0.05f+ (float)rand()/((float)RAND_MAX/0.1f); /* Random size        */
		float rotateX = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float rotateY = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float rotateZ = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float speed   = 0.1f + (float)rand()/((float)RAND_MAX/6.0f); /* Random speed      */

		Astroid * astroid = new Astroid("Astroid", glm::vec3(x,y,z), glm::vec3(scale), speed);
		astroid->orientation = glm::vec3(rotateX,rotateY,rotateZ);
		astroid->setMesh(mesh);
		astroid->setTexture(texture);
		astroid->init(shaders[NORMAL]);
		astroid->collisionDistance = 0.75f*scale;
		go.push_back(astroid);

	}
	
	halo = go.at(0);
	halo->collisionDistance = 3.0f;
	
	for(GameObject* g : go) {
		renderer->addObjects(g);
		octree->add(g);
	}
}
