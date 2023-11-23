#include "Debugger.h"

#include <GL/glew.h>
#include <iostream>

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall_(const char* function, const char* file, int line)
{
    while (auto err = glGetError())
    {
        std::cout << "[OpenGL]: " << err << ". Func: "
            << function << "; File: " << file << "; Line: " << line << '\n';
        return false;
    }
    return true;
}
