// This includes standard headers.
//#include <stdio.h>
//#include <stdlib.h>
//#include <vector>
//#include <iostream>
//
// This makes GLEW Static to avoid errors.
//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif
//
// This includes GLEW.
//#include <GL/glew.h>
//
// This includes GLFW.
// #include <glfw3.h>
//
// This includes GLM.
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;
//
// This inlcudes all of the 3D headers required.
//#include "shader.hpp"
//#include "texture.hpp"
//#include "controls.hpp"
//#include "objloader.hpp"
//#include "vboindexer.hpp"
//#include "Aela_Engine.h"
//#include "AelaWindow.h"
//#include "AelaError.h"
//#include "AelaModels.h"
//
// This is the functions that starts Aela and contains its loops.
//int startAela() {
//	 This is TEMPORARY and sets the window width and height.
//	int windowWidth = 1024, windowHeight = 768;
//	 This is also TEMPORARY and sets the window starting position.
//	int windowXPosition = 50, windowYPosition = 50;
//
//	AelaWindow window;
//	window.addProperty(AelaWindowFlag::AELA_WINDOW_SHOWN);
//	window.addProperty(AelaWindowFlag::AELA_WINDOW_OPENGL);
//	bool windowCreationSuccess = window.createWindow(windowWidth, windowHeight, windowXPosition, windowYPosition, "Aela Engine");
//	window.getWindowPosition(&windowXPosition, &windowYPosition);
//	std::cout << windowXPosition << " " << windowYPosition << "\n";
//
//	if (windowCreationSuccess == false) {
//		 Insert stuff here.
//		int temp, temp2;
//		window.getWindowDimensions(&temp, &temp2);
//		std::cout << temp << "\n";
//	} else {
//		window.makeWindowOpenGLContext();
//		window.hideCursor();
//	}
//
//	 Initialize GLEW.
//	glewExperimental = true; // Needed for core profile
//	if (glewInit() != GLEW_OK) {
//		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
//		getchar();
//		return -1;
//	}
//
//	 This gives us a dark blue background
//	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
//	 This enabes depth comparisons/depth testing.
//	glEnable(GL_DEPTH_TEST);
//
//	 This will accept a fragment if it closer to the camera than the previous one.
//	glDepthFunc(GL_LESS);
//
//	 This culls/gets triangles of which the normal is not towards the camera.
//	glEnable(GL_CULL_FACE);
//
//	 This is the vertex array buffer.
//	GLuint VertexArrayID;
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);
//
//	 This creates and compiles the GLSL program from the shaders.
//	GLuint depthProgramID = LoadShaders("shaders/DepthRTT.vertexshader", "shaders/DepthRTT.fragmentshader");
//
//	 This gets a handle for the "MVP" uniform.
//	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
//
//	 These are our models.
//	std::vector<AelaModel> models(2);
//	models[0].loadTexture("textures/uvmap.DDS");
//	models[1].loadTexture("textures/beretta.DDS");
//
//	 This loads the textures.
//	GLuint Texture = loadDDS("textures/uvmap.DDS");
//
//	 This laods the models from OBJ files.
//	models[0].loadModel("models/room_thickwalls.obj");
//	models[1].loadModel("models/beretta.obj");
//
//	 This will read our OBJ files and store their data.
//	std::vector<glm::vec3> vertices;
//	std::vector<glm::vec2> uvs;
//	std::vector<glm::vec3> normals;
//	std::string modelName = "models/room_thickwalls.obj";
//	bool res = loadOBJ(modelName, vertices, uvs, normals);
//
//	 This stores our indexed information.
//	std::vector<unsigned short> indices;
//	std::vector<glm::vec3> indexed_vertices;
//	std::vector<glm::vec2> indexed_uvs;
//	std::vector<glm::vec3> indexed_normals;
//	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
//
//	 This loads our buffers.
//	GLuint vertexbuffer;
//	glGenBuffers(1, &vertexbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
//	std::cout << indexed_uvs.size() * sizeof(glm::vec2) << " --\n";
//
//	GLuint uvbuffer;
//	glGenBuffers(1, &uvbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
//
//	GLuint normalbuffer;
//	glGenBuffers(1, &normalbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
//
//	GLuint elementbuffer;
//	glGenBuffers(1, &elementbuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
//
//	 This is the framebuffer (which regroups 0, 1 or more textures as well as 0 or 1 depth buffers).
//	GLuint FramebufferName = 0;
//	glGenFramebuffers(1, &FramebufferName);
//	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//
//	 This is a depth texture (for shadows), which is slower than a depth buffer but may be sampled later in a shader.
//	GLuint depthTexture;
//	glGenTextures(1, &depthTexture);
//	glBindTexture(GL_TEXTURE_2D, depthTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, windowWidth, windowWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
//
//	 There is no colour output in the bound framebuffer, only depth.
//	glDrawBuffer(GL_NONE);
//
//	 This is a safety check that makes sure that our frame buffer is okay.
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		return false;
//
//
//	 This is the quad's FBO and is used for (only) visualizing the shadowmap.
//	 *** I'm not necessarily sure as to how this works yet. *** //
//	static const GLfloat g_quad_vertex_buffer_data[] = {
//		-1.0f, -1.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,
//		-1.0f,  1.0f, 0.0f,
//		-1.0f,  1.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,
//		1.0f,  1.0f, 0.0f,
//	};
//
//	 This will bind our quad vertex buffer data.
//	GLuint quad_vertexbuffer;
//	glGenBuffers(1, &quad_vertexbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
//
//	 This will create and compile our GLSL program from the shaders.
//	GLuint quad_programID = LoadShaders("shaders/Passthrough.vertexshader", "shaders/SimpleTexture.fragmentshader");
//	GLuint texID = glGetUniformLocation(quad_programID, "texture");
//	GLuint programID = LoadShaders("shaders/ShadowMapping.vertexshader", "shaders/ShadowMapping.fragmentshader");
//
//	 "glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object". - khronos.org
//	 This gets a handle for the "myTextureSampler" uniform.
//	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
//
//	 This gets a handle for the "MVP" uniform.
//	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
//	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
//	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
//	GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
//	GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");
//
//	 This gets a handle for the "LightPosition" uniform.
//	GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");
//
//	 This is the program's running loop.
//	do {
//
//		window.updateWindowEvents();
//
//		 This says "render to our framebuffer".
//		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//		 This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
//		 Changing this can be used for split screen multiplayer gaming.
//		glViewport(0, 0, windowWidth, windowHeight);
//
//									   We don't use bias in the shader, but instead we draw back faces, 
//									   which are already separated from the front faces by a small distance 
//									   (if your geometry is made this way)
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
//
//							  Clear the screen
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		 This says "use this shader".
//		glUseProgram(depthProgramID);
//
//		glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);
//
//		 Compute the MVP matrix from the light's point of view
//		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
//		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//		 or, for spot light :
//		glm::vec3 lightPos(5, 20, 20);
//		glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
//		glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));
//
//		glm::mat4 depthModelMatrix = glm::mat4(1.0);
//		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
//
//		 Send our transformation to the currently bound shader, 
//		 in the "MVP" uniform
//		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
//
//		 1st attribute buffer : vertices
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glVertexAttribPointer(
//			0,  // The attribute we want to configure
//			3,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//		);
//
//		 Index buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//
//		 Draw the triangles !
//		glDrawElements(
//			GL_TRIANGLES,      // mode
//			indices.size(),    // count
//			GL_UNSIGNED_SHORT, // type
//			(void*)0           // element array buffer offset
//		);
//
//		glDisableVertexAttribArray(0);
//
//
//
//		 Render to the screen
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		glViewport(0, 0, windowWidth, windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
//
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
//
//							  Clear the screen
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		 Use our shader
//		glUseProgram(programID);
//
//		 Compute the MVP matrix from keyboard and mouse input
//		computeMatricesFromInputs(&window);
//		glm::mat4 ProjectionMatrix = getProjectionMatrix();
//		glm::mat4 ViewMatrix = getViewMatrix();
//		 ViewMatrix = glm::lookAt(glm::vec3(14,6,4), glm::vec3(0,1,0), glm::vec3(0,1,0));
//		glm::mat4 ModelMatrix = glm::mat4(1.0);
//		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
//
//		glm::mat4 biasMatrix(
//			0.5, 0.0, 0.0, 0.0,
//			0.0, 0.5, 0.0, 0.0,
//			0.0, 0.0, 0.5, 0.0,
//			0.5, 0.5, 0.5, 1.0
//		);
//
//		glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
//
//		 Send our transformation to the currently bound shader, 
//		 in the "MVP" uniform
//		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
//		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
//		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
//
//		glUniform3f(lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);
//
//		 Bind our texture in Texture Unit 0
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, Texture);
//		 Set our "myTextureSampler" sampler to user Texture Unit 0
//		glUniform1i(TextureID, 0);
//
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, depthTexture);
//		glUniform1i(ShadowMapID, 1);
//
//		 1rst attribute buffer : vertices
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glVertexAttribPointer(
//			0,                  // attribute
//			3,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//		);
//
//		 2nd attribute buffer : UVs
//		glEnableVertexAttribArray(1);
//		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//		glVertexAttribPointer(
//			1,                                // attribute
//			2,                                // size
//			GL_FLOAT,                         // type
//			GL_FALSE,                         // normalized?
//			0,                                // stride
//			(void*)0                          // array buffer offset
//		);
//
//		 3rd attribute buffer : normals
//		glEnableVertexAttribArray(2);
//		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//		glVertexAttribPointer(
//			2,                                // attribute
//			3,                                // size
//			GL_FLOAT,                         // type
//			GL_FALSE,                         // normalized?
//			0,                                // stride
//			(void*)0                          // array buffer offset
//		);
//
//		 Index buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//
//		 Draw the triangles !
//		glDrawElements(
//			GL_TRIANGLES,      // mode
//			indices.size(),    // count
//			GL_UNSIGNED_SHORT, // type
//			(void*)0           // element array buffer offset
//		);
//
//		glDisableVertexAttribArray(0);
//		glDisableVertexAttribArray(1);
//		glDisableVertexAttribArray(2);
//
//
//		 Optionally render the shadowmap (for debug only)
//
//		 Render only on a corner of the window (or we we won't see the real rendering...)
//		glViewport(0, 0, 512, 512);
//
//		// Use our shader
//		glUseProgram(quad_programID);
//
//		// Bind our texture in Texture Unit 0
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, depthTexture);
//		// Set our "renderedTexture" sampler to user Texture Unit 0
//		glUniform1i(texID, 0);
//
//		// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
//		//glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
//		glDisableVertexAttribArray(0);
//
//
//		 Swap buffers
//		window.updateBuffer();
//
//	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
//
//	 Cleanup VBO and shader
//	glDeleteBuffers(1, &vertexbuffer);
//	glDeleteBuffers(1, &uvbuffer);
//	glDeleteBuffers(1, &normalbuffer);
//	glDeleteBuffers(1, &elementbuffer);
//	glDeleteProgram(programID);
//	glDeleteProgram(depthProgramID);
//	glDeleteProgram(quad_programID);
//	glDeleteTextures(1, &Texture);
//
//	glDeleteFramebuffers(1, &FramebufferName);
//	glDeleteTextures(1, &depthTexture);
//	glDeleteBuffers(1, &quad_vertexbuffer);
//	glDeleteVertexArrays(1, &VertexArrayID);
//
//	 Close OpenGL window and terminate GLFW
//
//	return 0;
//}
//
//void Aela::start() {
//	startAela();
//}