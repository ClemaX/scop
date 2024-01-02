#include <scop.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include <vertex.h>
#include <shader.h>

#include <logger.h>

const char *scop_keystrtab[] =
{
	[KEY_ZOOM_IN] = "zoom in",
	[KEY_ZOOM_OUT] = "zoom out",
	[KEY_PAN_UP] = "pan up",
	[KEY_PAN_DOWN] = "pan down",
	[KEY_PAN_LEFT] = "pan left",
	[KEY_PAN_RIGHT] = "pan right",
	[KEY_ROTATE_UP] = "rotate up",
	[KEY_ROTATE_DOWN] = "rotate down",
	[KEY_ROTATE_LEFT] = "rotate left",
	[KEY_ROTATE_RIGHT] = "rotate right",
	[KEY_EXIT] = "exit",
};

static int		glfw_init(const scop_settings *settings)
{
	int		ret;
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));

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

		ret = chdir(cwd);

		if (ret != 0)
		{
			error("chdir: %s: %s\n", cwd, strerror(errno));
			glfwTerminate();
		}
	}
	else
		error("Failed to initialize GLFW\n");

	return ret;
}

static inline int	glew_init()
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

static void			on_resize(GLFWwindow* window, int width, int height)
{
	scop_scene	*scene;
	float	aspect;

	glViewport(0, 0, width, height);
	debug("Resized viewport to %dx%d!\n", width, height);

	scene = glfwGetWindowUserPointer(window);
	if (scene != NULL)
	{
		debug("Resizing scene...\n");
		scene->settings.width = width;
		scene->settings.height = height;

		aspect = (float)width / (float)height;

		debug("Setting camera projection for aspect ratio '%f'...\n", aspect);
		camera_set_aspect(&scene->cam, aspect);

		scop_draw(scene);
	}
}

static inline int	scop_window_init(scop_scene *scene)
{
	int	ret;

	debug("Initializing window...\n");
	scene->window = window_new(&scene->settings.width,
		&scene->settings.height, SCOP_WINDOW_NAME);

	ret = -(scene->window == NULL);
	if (ret == 0)
	{
		debug("Initialized window at %p\n", scene->window);

		glfwMakeContextCurrent(scene->window);
		glfwSetWindowUserPointer(scene->window, scene);
		//glViewport(0, 0, scene->settings.width, scene->settings.height);
	}

	return ret;
}

static inline int	scop_shader_init(scop_scene *scene)
{
	int	ret;

	scene->shader.id = shader_load(scene->settings.vertex_shader,
		scene->settings.fragment_shader);

	ret = -(scene->shader.id == 0);
	if (ret == 0)
	{
		scene->shader.mvp_loc = glGetUniformLocation(scene->shader.id,
			UNIFORM_MVP);

		scene->shader.res_loc = glGetUniformLocation(scene->shader.id,
			UNIFORM_RES);

		ret = -(scene->shader.mvp_loc == -1);
		if (ret != 0)
		{
			glDeleteShader(scene->shader.id);
			scene->shader.id = 0;
		}
	}

	return ret;
}

int			scop_init(scop_scene *scene)
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
				ret = scop_shader_init(scene);
				if (ret == 0)
				{
					const GLfloat	dist = 10;
					const vec3		target = { 0, 0, 0 };
					const vec3		up = { 0, 1, 0 };

					vertex_array_init(&scene->vao_id);
					vertex_buffer_init(&scene->vbo_id);
					vertex_buffer_init(&scene->vibo_id);

					camera_init(&scene->cam, 90.0f, 0.1f, 100.0f);
					camera_lookat(&scene->cam, up, target, dist);

					glEnable(GL_CULL_FACE);

					glfwSetWindowSizeCallback(scene->window, &on_resize);

					glfwGetWindowSize(scene->window, &scene->settings.width,
						&scene->settings.height);
				}
			}

			if (ret != 0)
			{
				error("Fatal error, destroying window!\n");
				glfwDestroyWindow(scene->window);
				scene->window = NULL;
			}
		}
		if (ret != 0)
			glfwTerminate();
	}

	return ret;
}
