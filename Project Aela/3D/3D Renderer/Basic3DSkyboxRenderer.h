/*
* Name: Project Aela's Basic 3D Skybox Renderer
* Author: Robert Ciborowski
* Date: 21/06/2017
* Description: A class used by Project Aela's Basic 3D Renderer to render a skybox using a cubemap.
*/

#include "../Skybox/Skybox.h"
#include "../3D Camera/Camera3D.h"

class Basic3DSkyboxRenderer {
	public:
		Basic3DSkyboxRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderSkybox(Skybox* skybox, GLuint skyboxProgramID, GLuint frameBuffer, GLuint skyboxID, GLuint modelViewMatrixID, GLuint projectionMatrixID);
		void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

	private:
		glm::mat4 viewMatrix, projectionMatrix;
};