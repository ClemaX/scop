#pragma once

#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#define WIN_FULLSCREEN -1
#define WIN_FULLSCREEN_WINDOWED -2

GLFWwindow	*window_new(int *width, int *height, const char *name);

int			window_loop(GLFWwindow *window);
