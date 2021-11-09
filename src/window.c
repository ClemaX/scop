#include <window.h>

#include <logger.h>

GLFWmonitor	*monitor_get(const GLFWvidmode **mode)
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

GLFWwindow	*window_new(int *width, int *height, const char *name)
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

int			window_loop(GLFWwindow *window)
{
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			0,			// attribute
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized
			0,			// stride
			(void*)0	// array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(window) == 0);

	return 0;
}
