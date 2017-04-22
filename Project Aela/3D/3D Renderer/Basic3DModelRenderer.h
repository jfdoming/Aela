/*
* Class: Basic 3D Model Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to specifically render 3D models.
*/

#include "../../3D/Models/Models.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Window/Window.h"

class Basic3DModelRenderer {
	public:
		Basic3DModelRenderer() {
		}

		// This function renders a model. It requires a lot of GLuints that are provided by
		// the Basic3DRenderer.
		void renderModel(Model3D* model, GLuint frameBuffer, GLuint programID, GLuint depthMatrixID,
			GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID,
			GLuint textureID, GLuint depthTexture, GLuint shadowMapID);
		// This function renders a 2D texture in 3D space. It requires a lot of GLuints that
		//  are provided by the Basic3DRenderer.
		void renderTextureIn3DSpace(Window* window, bool cullFaces, GLuint texture, GLuint textureID,
			GLuint programID, GLuint frameBuffer, GLuint viewMatrixID, GLuint matrixID, GLuint modelMatrixID,
			GLuint depthBiasID, GLuint depthTexture, GLuint shadowMapID, GLuint lightInvDirID, GLuint depthMatrixID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		// This is made for the Basic3DRenderer to set matrices.
		void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

	private:
		glm::mat4 biasMatrix = glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);
		glm::mat4 viewMatrix, projectionMatrix;

		void drawTestQuad();
};