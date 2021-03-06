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
	double startTime = 0.0f;
	float pie = 3.1415f;
	selected = nullptr;
	control = new Controls();
}

Game::~Game() {

}

void Game::build() {
	shaders[NORMAL]		= LoadShaders( "src/shaders/StandardShading.vertexshader", "src/shaders/StandardShading.fragmentshader" );
	handles[MATRIXID]	= glGetUniformLocation(shaders[NORMAL], "MVP");
	TextureIDs  = glGetUniformLocation(shaders[NORMAL], "myTextureSampler");
	
	buildGameObjects();
}

void Game::run() {
	// For speed computation
	double lastTime = Time::getInstance().getTime();
	double lastFrameTime = lastTime;
	double shootTime = 2.0f;

	int nbFrames = 0;

	initText2D( "src/resources/Holstein.tga" );
	
	Mesh * mesh2 = new Mesh("src/resources/rocket.obj");
	Texture * texture2 = new Texture("src/resources/white.bmp");

	do{
		// Measure speed
		double currentTime = Time::getInstance().getTime();
		float deltaTime = (float)(currentTime - lastFrameTime); 
		lastFrameTime = currentTime;
		shootTime += deltaTime;
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

		glUseProgram(shaders[NORMAL]);

		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(handles[MATRIXID], 1, GL_FALSE, &MVP[0][0]);

		halo->translateObject(control->getPosition());
		halo->rotateObject(control->getDirection());
		
		std::vector<Rocket*>::iterator iter = rockets.begin();

		while (iter != rockets.end())
		{
			(*iter)->duration -= deltaTime;

			glm::mat4 RotationMatrix		= eulerAngleYXZ((*iter)->orientation.x, (*iter)->orientation.y, (*iter)->orientation.z);
			glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
			glm::vec3 realforward(forward);
			(*iter)->position -= realforward * deltaTime * 7.0f;

			if ((*iter)->duration <= 0) {
				renderer->removeObject((*iter));
				octree->removeObject((*iter));
				delete *iter;
				iter = rockets.erase(iter);
			} else
			   ++iter;
		}

		std::vector<GameObject*>::iterator it;

		for(it = renderer->gameObjects.begin()+1; it != renderer->gameObjects.end(); ++it)
		{
			Astroid * anAstroid = dynamic_cast<Astroid*>(*it);
			if( anAstroid != 0 ) {
				if((*it)->name != "HaloCharacter") {
					glm::mat4 RotationMatrix		= eulerAngleYXZ((anAstroid)->orientation.x, (anAstroid)->orientation.y, (anAstroid)->orientation.z);
					glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
					glm::vec3 realforward(forward);
					anAstroid->position -= realforward * deltaTime * anAstroid->speed * 0.5f;
				}
			}
		}

		if(glfwGetKey('F') == GLFW_PRESS) {
			if(shootTime > 0.5f) {
				shootTime = 0.0f;
				rocket = new Rocket("Rocket", control->getPosition(), glm::vec3(0.2f), 5.0f);
				rocket->orientation = control->carDirection;
				rocket->setMesh(mesh2);
				rocket->setTexture(texture2);
				rocket->init(shaders[NORMAL]);
				rocket->collisionDistance = 1.0f;
				renderer->addObjects(rocket);
				rockets.push_back(rocket);
				octree->add(rocket);
			}
		}

		renderer->renderObjects(ProjectionMatrix, ViewMatrix, handles[MATRIXID]);
		
		std::vector<GameObject*> meuk;
		octree->gatherObjects(meuk);
		delete octree;
		octree = new Octree(this, glm::vec3(0.0f), 55.0f, 3);
		std::vector<GameObject*>::iterator myiter;
		
		for(myiter = meuk.begin(); myiter != meuk.end(); ++myiter) {
			octree->add(*myiter);
		}
		
		for(GameObject* go : collisionObjects) {
			renderer->removeObject(go);
		}

		collisionObjects.clear();

		octree->CheckEdges();		
		octree->detectCollisions();

		if(glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
			Astroid * a = dynamic_cast<Astroid*>( GetGameObjectFromPosition(control->getMousePosition()));
			if ( a != nullptr )
				selected = a;
		}
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

		sprintf(text,"%d Astroids left!", renderer->gameObjects.size() - 1 - rockets.size()); //because halo and rockets
		printText2D(text, 10,580, 16);
		
		if(selected != nullptr) {
			sprintf(text,"Astroid%d",selected->id);
			printText2D(text, 550, 580, 15);
			sprintf(text,"Size: %.2f",selected->scaling.x);
			printText2D(text, 550, 565, 15);
			float distance = glm::distance(selected->position, halo->position);
			sprintf(text,"Distance: %.2f",distance);
			printText2D(text, 550, 550, 15);
		
		}

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
	//glfwDisable(GLFW_MOUSE_CURSOR);
	glfwSetMousePos(1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Game::setGlParameters() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LESS); 
 
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(1);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Game::buildGameObjects() {
	int min = -25;
	int max = 50;
	octree = new Octree(this, glm::vec3(0), 55.0f, 3);
	Director dir;
	ObjectBuilder ob;
	dir.setBuilder(&ob);
	vector<GameObject*> go = dir.getGameObject(shaders[NORMAL]);
	
	renderer = new Renderer();

	Mesh * mesh = new Mesh("src/resources/ball.obj");
	Texture * texture = new Texture("src/resources/land.bmp");
	explosionObject = new Mesh("src/resources/ball.obj");
	collisionTexture = new Texture("src/resources/white.bmp");

	for ( int i = 0; i < 40; ++i ) {		
		float x       = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float y		  = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float z		  = min  + (float)rand()/((float)RAND_MAX/max); /* Random position	 */
		float scale   = 0.2f + (float)rand()/((float)RAND_MAX/1.0f); /* Random size        */
		float rotateX = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float rotateY = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float rotateZ = 0.0  + (float)rand()/((float)RAND_MAX/(3.1415*180)); /* Random orientation */
		float speed   = 0.1f + (float)rand()/((float)RAND_MAX/1.0f); /* Random speed      */

		Astroid * astroid = new Astroid("Astroid", glm::vec3(x,y,z), glm::vec3(scale), 0);
		astroid->orientation = glm::vec3(rotateX,rotateY,rotateZ);
		astroid->setMesh(mesh);
		astroid->setTexture(texture);
		astroid->init(shaders[NORMAL]);
		astroid->collisionDistance = scale / 2.2f;
		astroid->id = i;
		go.push_back(astroid);
	}

	halo = go.at(0);
	halo->collisionDistance = 1.0f;
	control->spaceShip = halo;
	for(GameObject* g : go) {
		renderer->addObjects(g);
		octree->add(g);
	}
}

GameObject * Game::GetGameObjectFromPosition(glm::vec3 position){
	float shortestDistance = 10.0f;
	Astroid * go = nullptr;
	for(GameObject * g: renderer->gameObjects) {
		float distanceBetween = glm::distance(g->getPosition(), position);
		//std::cout << distanceBetween << std::endl;
		if(distanceBetween < shortestDistance && distanceBetween < g->collisionDistance) {
			shortestDistance = distanceBetween;
			Astroid * a = dynamic_cast<Astroid*>(g);
			if ( a != nullptr ) {
				go = a;
			}
        }
    }
	return go;
}

void Game::addCollisionObject(glm::vec3 pos) {
	collisionObject = new GameObject("CollisionObject", pos, glm::vec3(0.08f));
	collisionObject->setMesh(explosionObject);
	collisionObject->setTexture(collisionTexture);
	collisionObject->init(shaders[NORMAL]);

	collisionObjects.push_back(collisionObject);
	renderer->addObjects(collisionObject);
}

void Game::removeObject(GameObject* gameObject) {
	std::vector<Rocket*>::iterator iter = rockets.begin();

	while (iter != rockets.end())
	{
		if ((*iter) == gameObject) {
			iter = rockets.erase(iter);
			break;
		} else
			++iter;
	}
}