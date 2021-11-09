#include <GL/glew.h>

#include <scop.h>
#include <vertex.h>

#include <logger.h>

#include <shader.h>

static int	glfw_init(const scop_settings *settings)
{
	// Needed for core profile
	glewExperimental = GL_TRUE;

	if (glfwInit() == GL_FALSE)
	{
		error("Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, settings->anti_aliasing); // 4x Anti Aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings->gl_major); // We want OpenGL 4.X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings->gl_minor);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE); // Do not minimize fullscreen applications on focus loss
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	//glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	//glfwWindowHint(GLFW_FOCUS_ON_SHOW, GL_TRUE); // Specifies whether the window will be given input focus when glfwShowWindow is called.

	return 0;
}

static int	glew_init()
{
	// Needed for core profile
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		error("Failed to initialize GLEW\n");
		return -1;
	}

	return 0;
}

static void	on_resize(GLFWwindow* window, int width, int height)
{
	(void)window;
	(void)width;
	(void)height;

	glViewport(0, 0, width, height);
	debug("Resized window to %dx%d!\n", width, height);
}

GLFWwindow	*scop_init(scop_settings *settings, GLuint *shader_program_id)
{
	GLFWwindow	*window;

	if (glfw_init(settings) == 0)
	{
		window = window_new(&settings->width, &settings->height, SCOP_WINDOW_NAME);

		if (window != NULL)
		{
			glfwSetWindowSizeCallback(window, &on_resize);
			glfwMakeContextCurrent(window);

			//glfwShowWindow(window);
			//glfwFocusWindow(window);
			// TODO: Find a way to focus full-screen window in macos

			if (glew_init() == 0)
			{
				debug("Renderer: %s\nOpenGL version: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

				*shader_program_id = shader_load(SHADER_DIR"/vertex.glsl", SHADER_DIR"/fragment.glsl");

				if (*shader_program_id != 0)
					return window;
			}
		}
		glfwTerminate();
	}

	return NULL;
}

void		scop_terminate()
{
	glfwTerminate();
}


static void	triangle_draw()
{
	static const GLfloat	vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int			scop_loop(GLFWwindow *window, GLuint shader_program_id)
{
	GLuint	vao;
	GLuint	vb;
	int		ret;

	vertex_array_object(&vao);
	vertex_buffer(&vb);

	triangle_draw();

	ret = window_loop(window, shader_program_id);

	scop_terminate();

	return ret;
}
