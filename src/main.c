#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
# define info(...) fprintf(stderr, __VA_ARGS__)
# define error(...) fprintf(stderr, __VA_ARGS__)
# define WINDOW_NAME "scop (debug)"
#else
# define debug(...) (void)0
# define info(...) fprintf(stderr, __VA_ARGS__)
# define error(...) fprintf(stderr, __VA_ARGS__)
# define WINDOW_NAME "scop"
#endif

typedef struct	scop_settings
{
	int	anti_aliasing;
}				scop_settings;

int			scop_init(const scop_settings *settings)
{
	// Needed for core profile
	glewExperimental = true;
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, settings->anti_aliasing); // 4x Anti Aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	return 0;	
}

GLFWwindow	*window_new(int width, int height, const char *name)
{
	GLFWwindow	*window;

	window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window != NULL)
	{
		// Ensure we can capture the escape key being pressed
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	}
	else
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		perror("test");
		glfwTerminate();
	}

	return window;
}

int			window_loop(GLFWwindow *window)
{
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Draw something

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(window) == 0);

	return 0;
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;

	static const scop_settings	settings = (scop_settings){ .anti_aliasing = 4 };
	GLFWwindow					*window;

	if (scop_init(&settings) == -1)
		return -1;

	window = window_new(1024, 768, WINDOW_NAME);

	if (window != NULL)
		window_loop(window);

	return 0;
}
