#include <GL/glew.h>

#include <scop.h>
#include <vertex.h>

#include <logger.h>

#include <shader.h>

static int	glfw_init(const scop_settings *settings)
{
	int	ret;

	// Needed for core profile
	glewExperimental = GL_TRUE;

	ret = -(glfwInit() == GL_FALSE);

	if (ret == 0)
	{
		glfwWindowHint(GLFW_SAMPLES, settings->anti_aliasing); // 4x Anti Aliasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings->gl_major); // We want OpenGL 4.X
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings->gl_minor);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE); // Do not minimize fullscreen applications on focus loss
#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	}
	else
		error("Failed to initialize GLFW\n");

	return ret;
}

static int	glew_init()
{
	int	ret;

	// Needed for core profile
	glewExperimental = GL_TRUE;

	ret = -(glewInit() != GLEW_OK);
	if (ret == 0)
		debug("Renderer: %s\nOpenGL version: %s\n",
			glGetString(GL_RENDERER), glGetString(GL_VERSION));
	else
		error("Failed to initialize GLEW\n");

	return ret;
}

static void	on_resize(GLFWwindow* window, int width, int height)
{
	(void)window;
	(void)width;
	(void)height;

	glViewport(0, 0, width, height);
	debug("Resized window to %dx%d!\n", width, height);
}

static int	scop_window_init(scop *scene)
{
	int	ret;

	scene->window = window_new(&scene->settings.width,
		&scene->settings.height, SCOP_WINDOW_NAME);

	ret = -(scene->window == NULL);
	if (ret == 0)
	{
		glfwSetWindowSizeCallback(scene->window, &on_resize);
		glfwMakeContextCurrent(scene->window);
	}

	return ret;
}

int	scop_init(scop *scene)
{
	int			ret;

	ret = glfw_init(&scene->settings);
	if (ret == 0)
	{
		ret = scop_window_init(scene);
		if (ret == 0)
		{
			ret = glew_init();
			if (ret == 0)
			{
				scene->shader_id = shader_load(SHADER_DIR"/vertex.glsl",
					SHADER_DIR"/fragment.glsl");
				ret = -(scene->shader_id == 0);
			}

			if (ret != 0)
			{
				glfwDestroyWindow(scene->window);
				scene->window = NULL;
			}
		}
		if (ret != 0)
			glfwTerminate();
	}

	return ret;
}

void		scop_terminate()
{
	glfwTerminate();
}


static void	triangle_draw()
{
	static GLfloat	vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int			scop_loop(scop *scene)
{
	GLuint	vao;
	GLuint	vb;

	vertex_array_object(&vao);
	vertex_buffer(&vb);

	triangle_draw();

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(scene->shader_id);

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
		glfwSwapBuffers(scene->window);
		glfwPollEvents();
	} while (glfwGetKey(scene->window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(scene->window) == 0);

	scop_terminate();

	return 0;
}
