#include "AelaModels.h"

bool AelaModel::loadModel(std::string path) {
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	bool res = loadOBJ(path, tempVertices, tempUVs, tempNormals);
	if (res) {
		indexVBO(tempVertices, tempUVs, tempNormals, indices, vertices, uvs, normals);
		return true;
	} else {
		return false;
	}
}

bool AelaModel::loadTexture(std::string path) {
	texture = loadDDS(path);
	return (texture != NULL);
}

void AelaModel::setPosition(float setX, float setY, float setZ) {
	xPosition = setX;
	yPosition = setY;
	zPosition = setZ;
	position = glm::vec3(setX, setY, setZ);
}

glm::vec3 AelaModel::getPosition() {
	return position;
}

void AelaModel::getPosition(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xPosition;
	*setYPosition = yPosition;
	*setZPosition = zPosition;
}

glm::vec3 * AelaModel::getVertices() {
	return &vertices[0];
}

glm::vec2 * AelaModel::getUVs() {
	return &uvs[0];
}

glm::vec3 * AelaModel::getNormals() {
	return &normals[0];
}

unsigned short * AelaModel::getIndices() {
	return &indices[0];
}

int AelaModel::getVertexSize() {
	return vertices.size();
}

int AelaModel::getUVSize() {
	return uvs.size();
}

int AelaModel::getNormalSize() {
	return normals.size();
}

int AelaModel::getIndexSize() {
	return indices.size();
}

GLuint * AelaModel::getTexture() {
	return &texture;
}