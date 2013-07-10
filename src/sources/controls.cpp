#include "../headers/Controls.h"

Controls::Controls() {
	position = glm::vec3( 0,0.1f, -0.15f ); 
	carPosition = glm::vec3(0.0f, 0.0f, 0.0f );
	carDirection = glm::vec3(0,0,0);

	horizontalAngle = 3.1415f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;
	rotationSpeed = 0.0f;
	speedPerTick = 1.0f;
	acceleration = 0.0f;
	steering = 0.0f;

	speed = 3.0f; 
	mouseSpeed = 0.005f;
}

void Controls::updateCamera(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	float FoV = initialFoV - 5;

	glm::vec3 temp = carDirection;

	temp.x = carDirection.y;
	temp.y = -carDirection.x;
	temp.z = carDirection.z;

	glm::vec3 lookAt = carPosition;

	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	ViewMatrix       = glm::lookAt(
								carPosition + position * glm::quat(temp), 	// Camera is here
								lookAt, 									// and looks here : at the same position, plus "direction"
								vec3(0, 1, 0)                  				// Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Controls::update(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if(glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS) {
		steering += 0.2f;

		if(steering > 1.5f) 
			steering = 0.7f;
	} else if(glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS) {
		steering -= 0.2f;

		if(steering < -1.5f) 
			steering = -0.7f;
	} else {
		steering = 0;
	}
	
	carDirection += vec3(glm::radians(steering), 0, 0);

	glm::mat4 RotationMatrix		= eulerAngleYXZ(carDirection.x, carDirection.y, carDirection.z);
	glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
	glm::vec3 realforward(forward);

	if(glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS) {
		if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS) {
			rotationSpeed += speedPerTick;
			acceleration += speedPerTick * 0.06f;

			if(acceleration > 1)
				acceleration = 1;

			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
			rotationSpeed -= speedPerTick;
			acceleration -= speedPerTick * 0.06f;

			if(acceleration < -1)
				acceleration = -1;

			carPosition -= realforward * deltaTime * speed * acceleration;
		}
	} else {
		if(acceleration > 0.1f) {
			acceleration -= speedPerTick * 0.3f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		else if (acceleration < -0.1f){
			acceleration += speedPerTick * 0.3f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		} else
			acceleration = 0;
	}

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


glm::mat4 Controls::getViewMatrix(){
	return ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::vec3 Controls::getCameraPosition() {
	return position;
}

float Controls::getSteering() { 
	return steering;
}

float Controls::getRotationSpeed() {
	return rotationSpeed;
}

glm::vec3 Controls::getPosition() {
	return carPosition;
}

glm::vec3 Controls::getDirection() {
	return carDirection;
}

float Controls::getSpeed() {
	return acceleration;
}

void Controls::setSpeed(float accel) {
	acceleration = accel;
}

void Controls::accept(Visitor &v) {
	v.visit(this);
}