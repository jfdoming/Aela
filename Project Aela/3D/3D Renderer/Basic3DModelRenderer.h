/*
* Class: Basic 3D Model Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to specifically render 3D models.
*/

#include "../../3D/Models/Models.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Window/Window.h"
#include "../3D Light/Light3D.h"

class Basic3DModelRenderer {
	public:
		Basic3DModelRenderer() {
		}

		// This function renders a model. It requires a lot of GLuints that are provided by
		// the Basic3DRenderer.
		void renderModel(Model3D* model, GLuint frameBuffer, GLuint modelProgramID, GLuint modelMVPMatrixID, GLuint modelMatrixID,
			GLuint modelViewMatrixID, GLuint modelTextureID, GLuint cameraPositionID, glm::vec3* cameraPosition);

		// This function renders a 2D texture in 3D space.
		void renderTextureIn3DSpace(bool cullFaces, GLuint texture, GLuint billboardTextureID,
			GLuint programID, GLuint frameBuffer, GLuint billboardMVPMatrixID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);

		// This is made for the Basic3DRenderer in order to set matrices.
		void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

		// This sends light data to the model shader.
		void sendLightDataToShader(std::vector<Light3D>* lights, GLuint modelProgramID, GLuint numberOfLightsID, GLuint lightPositionsID,
			GLuint lightDirectionsID, GLuint lightColoursID, GLuint lightPowersID, GLuint shadowMapID);

	private:
		glm::mat4 biasMatrix = glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);
		glm::mat4 viewMatrix, projectionMatrix;
		const unsigned int MAX_LIGHT_AMOUNT = 5;

		void drawTestQuad();
		void renderTestCube();
};