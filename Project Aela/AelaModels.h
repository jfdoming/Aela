#include <vector>
#include <string>
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "objloader.hpp"
#include "vboindexer.hpp"
#include "texture.hpp"

class AelaModel {
	private:
		float xPosition, yPosition, zPosition;
		glm::vec3 position;
	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> indices;
		GLuint texture;

		AelaModel() {
			xPosition = 0;
			yPosition = 0;
			zPosition = 0;
			position = glm::vec3(0.0, 0.0, 0.0);
		}

		~AelaModel() {
			glDeleteTextures(1, &texture);
		}

		bool loadModel(std::string path);
		bool loadTexture(std::string path);
		void setPosition(float setX, float setY, float setZ);
		glm::vec3 getPosition();
		void getPosition(float * setXPosition, float * setYPosition, float * setZPosition);
		glm::vec3 * getVertices();
		glm::vec2 * getUVs();
		glm::vec3 * getNormals();
		unsigned short * getIndices();
		int getVertexSize();
		int getUVSize();
		int getNormalSize();
		int getIndexSize();
		GLuint * getTexture();
};