#include "../headers/GameObject.h"

GameObject::GameObject() {

}

GameObject::GameObject(std::string _name, glm::vec3 aPosition, glm::vec3 scale, bool steer, glm::vec3 rotation) {
	name = _name;
	steerable = steer;
	
	_parent = NULL;
	tireBehaviour = false;
	rotationSpeed = 0.0f;

	rotateObject(rotation);
	translateObject(aPosition);
	scaleObject(scale);
}

GameObject::~GameObject() {
	
}

void GameObject::translateObject( glm::vec3 translation )
{
	position = translation;
}

void GameObject::scaleObject(glm::vec3 scale) {
	scaling = scale;
}

void GameObject::rotateObject(glm::vec3 axis ) {
	orientation = axis;
}

const std::string GameObject::getName() {
	return name;
}

glm::vec3 GameObject::getOrientation() {
	return orientation;
}

glm::quat GameObject::getOrientationQuat() {
	glm::vec3 temp;
	temp.x = _parent->getOrientation().y;
	temp.y = _parent->getOrientation().x;
	temp.z = _parent->getOrientation().z;

	return glm::quat(temp);
}

glm::vec3 GameObject::getScaling() {
	return scaling;
}

glm::vec3 GameObject::getPosition() {
	return position;
}

GameObject* GameObject::getParent() {
	return _parent;
}

glm::vec3 GameObject::getSteering() {
	return steering;
}

float GameObject::getRotationSpeed() {
	return rotationSpeed;
}

void GameObject::init(GLuint shader) {
	// Get a handle for our buffers
	vertexPosition_modelspaceID	= glGetAttribLocation(shader, "vertexPosition_modelspace");
	vertexUVID					= glGetAttribLocation(shader, "vertexUV");
	vertexNormal_modelspaceID	= glGetAttribLocation(shader, "vertexNormal_modelspace");
}

void GameObject::setTexture(Texture& tex) {
	colorMap = &tex;
}

void GameObject::setTexture(Texture* tex) {
	colorMap = tex;
}

void GameObject::setMesh(Mesh* mes) {
	mesh = mes;
}

void GameObject::setParent(GameObject& gameObject) {
	_parent = &gameObject;
}

void GameObject::setTireBehaviour() {
	tireBehaviour = true;
}

void GameObject::setRotationSpeed(float speed) {
	rotationSpeed = speed;
}

void GameObject::setSteering(glm::vec3 steer) { 
	steering = steer;
}

bool GameObject::hasParent() {
	return (_parent != NULL);
}

bool GameObject::hasTireBehaviour() {
	return tireBehaviour;
}

bool GameObject::isSteering() {
	return steerable;
}

void GameObject::render() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorMap->getTexture());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->verticesBuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID, 
		3,                            
		GL_FLOAT,                    
		GL_FALSE,                     
		0,                            
		(void*)0                      
	);
 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvsBuffer);
	glVertexAttribPointer(
		vertexUVID,                 
		2,                          
		GL_FLOAT,                 
		GL_FALSE,                 
		0,                         
		(void*)0              
	);
 
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalsBuffer);
	glVertexAttribPointer(
		vertexNormal_modelspaceID,  
		3,                          
		GL_FLOAT,               
		GL_FALSE,                  
		0,                     
		(void*)0               
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesBuffer);

	glDrawElements(
		GL_TRIANGLES,      
		mesh->vertices.size(),  
		GL_UNSIGNED_SHORT,  
		(void*)0          
	);
}

void GameObject::accept(Visitor &v) {
	v.visit(this);
}