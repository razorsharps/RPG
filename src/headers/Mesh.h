#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
 
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "../headers/Texture.h"
#include "../headers/objloader.h"
#include "../headers/vboindexer.h"

class Mesh
{
	private: // data mambers

	public: // functions
		Mesh();
		Mesh(const char * fileName);
		virtual ~Mesh();

		void load( const char * fileName );

		std::vector< unsigned short > indices; // index to the v/n/t/u/
		std::vector< glm::vec3 > vertices; // all the vertices
		std::vector< glm::vec3 > normals; // normal to the vertex
		std::vector< glm::vec2 > uvs; // uv to the vertex

		GLuint indicesBuffer;	// id of buffered mesh;
		GLuint verticesBuffer;	// id of buffered mesh;
		GLuint normalsBuffer;	// buffered normals id
		GLuint uvsBuffer;		// buffered uv's id

		unsigned int size();
};
