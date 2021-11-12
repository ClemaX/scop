#include <window.h>

#include <logger.h>

static GLFWmonitor	*monitor_get(const GLFWvidmode **mode)
{
	GLFWmonitor *const			monitor = glfwGetPrimaryMonitor();

	if (monitor != NULL)
	{
		if (mode != NULL)
			*mode = glfwGetVideoMode(monitor);
	}
	else
		error("Could not get the primary GLFW monitor!\n");

	return monitor;
}

GLFWwindow			*window_new(int *width, int *height, const char *name)
{
	GLFWwindow			*window;
	GLFWmonitor			*monitor;
	const int			type = *width;

	if (*width < 0)
	{
		const GLFWvidmode	*mode;

		monitor = monitor_get(&mode);

		if (monitor == NULL)
			return NULL;

		*width = mode->width;
		*height = mode->height;

		if (type == WIN_FULLSCREEN_WINDOWED)
		{
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		}

		debug("Creating fullscreen%s window '%s' [%dx%d]...\n",
			type == -1 ? "" : "/windowed", name, *width, *height);
	}
	else
	{
		monitor = NULL;
		debug("Creating window '%s' [%dx%d]...\n", name, *width, *height);
	}

	window = glfwCreateWindow(*width, *height, name, monitor, NULL);
	if (window != NULL)
	{
		// Ensure we can capture the escape key being pressed
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	}
	else
	{
		error("Failed to open GLFW window.\n");
		glfwTerminate();
	}

	return window;
}
