#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
	if (!glfwInit())
	{
		error("Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, settings->anti_aliasing); // 4x Anti Aliasing
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.X
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Needed for core profile
	glewExperimental = GL_TRUE;
	
	if (!glewInit())
	{
		error("Failed to initialize GLEW\n");
		return -1;
	}

#ifdef DEBUG
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
#endif
	return 0;	
}

GLFWwindow	*window_new(int width, int height, const char *name)
{
	GLFWwindow	*window;

	debug("Creating window '%s' [%dx%d]...\n", name, width, height);
	window = glfwCreateWindow(width, height, name, NULL, NULL);
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

void		vertex_array_object(GLuint *vertex_array_id)
{
	debug("Initializing vertex array object...\n");
	glGenVertexArrays(1, vertex_array_id);
	debug("Binding vertex array object with id %u...\n", *vertex_array_id);
	glBindVertexArray(*vertex_array_id);
	debug("Initialized vertex array object with id %u\n", *vertex_array_id);
}

void		vertex_buffer(GLuint *vertex_buffer_id)
{
	debug("Initializing vertex buffer...\n");
	glGenBuffers(1, vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer_id);
	debug("Initialized vertex buffer with id %u\n", *vertex_buffer_id);
	
}
void		triangle_draw()
{
	static const GLfloat	vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int			main(int ac, char **av)
{
	(void)ac;
	(void)av;

	static const scop_settings	settings = (scop_settings){ .anti_aliasing = 4 };
	GLFWwindow					*window;
	GLuint						vao;
	GLuint						vb;

	if (scop_init(&settings) == -1)
		return -1;

	window = window_new(1024, 768, WINDOW_NAME);

	if (window != NULL)
	{
		vertex_array_object(&vao);
		vertex_buffer(&vb);

		triangle_draw();

		window_loop(window);
	}
	glfwTerminate();
	return 0;
}
