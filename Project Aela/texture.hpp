#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
// GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, () has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(std::string filePath);

// This function should probably be moved somewhere else. Maybe it should be moved into
// a header file called "useful_functions.h". For now, it exists here.
std::string getPartOfString(std::string string, unsigned int start, unsigned int length);


#endif
