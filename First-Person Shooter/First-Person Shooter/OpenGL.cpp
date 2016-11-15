#include "OpenGL.h"

OpenGL::OpenGL()
{
	hRC = NULL;
}

OpenGL::~OpenGL()
{
	CleanUp();
}
