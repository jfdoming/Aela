#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <string.h>

#ifndef SHADER_HPP
#define SHADER_HPP

GLuint loadShaders(std::string vertexShaderPath, std::string fragmentShaderPath);

#endif

