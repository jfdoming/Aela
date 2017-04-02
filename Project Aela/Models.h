/*
* Name: Project Aela's Model Class
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a model.
*/

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

#ifndef AELA_MODEL_3D_PROPERTY
#define AELA_MODEL_3D_PROPERTY
// This enum is used in an Model3D function to change a single property.
enum class Model3DProperty {
	X_POSITION, Y_POSITION, Z_POSITION,
	X_ROTATION, Y_ROTATION, Z_ROTATION
};
#endif

class Model3D {
	private:
		// Translation.
		glm::vec3 position;
		// Rotation.
		glm::vec3 rotation;

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> indices;
		GLuint texture;

		Model3D() {
			position = glm::vec3(0.0, 0.0, 0.0);
			rotation = glm::vec3(0.0, 0.0, 0.0);
		}

		~Model3D() {
			glDeleteTextures(1, &texture);
		}

		bool loadModel(std::string path);
		bool loadTexture(std::string path);

		// Positioning getters and setters.
		void setPosition(float setX, float setY, float setZ);
		void setPosition(glm::vec3 setPosition);
		glm::vec3 getPosition();
		void getPosition(float* setXPosition, float* setYPosition, float* setZPosition);

		// Rotation getters and setters.
		void setRotation(float setX, float setY, float setZ);
		void setRotation(glm::vec3 setRotation);
		glm::vec3 getRotation();
		void getRotation(float* setXPosition, float* setYPosition, float* setZPosition);

		// A function used for changing any single transformation-related property.
		void setProperty(Model3DProperty property, float value);
		float getProperty(Model3DProperty property);

		glm::vec3* getVertices();
		glm::vec2* getUVs();
		glm::vec3* getNormals();
		unsigned short* getIndices();
		int getVertexSize();
		int getUVSize();
		int getNormalSize();
		int getIndexSize();
		GLuint* getTexture();
};