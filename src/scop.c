#include <GL/glew.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include <scop.h>
#include <vertex.h>
#include <shader.h>

#include <logger.h>

static int	glfw_init(const scop_settings *settings)
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

int			scop_init(scop *scene)
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
				if (ret == 0)
				{
					camera_init(&scene->cam, 90.0f, 1.0f, 100.0f);
				}
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

void		scop_terminate(scop *scene)
{
	if (scene->shader_id != 0)
	{
		glDeleteShader(scene->shader_id);
		scene->shader_id = 0;
	}
	glfwTerminate();
	object_destroy(&scene->obj);
}

int	scene_movement(scop *scene)
{
	vec3	velocity = { 0, 0, 0 };
	int		moved;

	if (glfwGetKey(scene->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return -1;

	if (glfwGetKey(scene->window, GLFW_KEY_W) == GLFW_PRESS)
		velocity[z] += SCOP_VELOCITY;
	if (glfwGetKey(scene->window, GLFW_KEY_S) == GLFW_PRESS)
		velocity[z] -= SCOP_VELOCITY;

	if (glfwGetKey(scene->window, GLFW_KEY_A) == GLFW_PRESS)
		velocity[x] += SCOP_VELOCITY;
	if (glfwGetKey(scene->window, GLFW_KEY_D) == GLFW_PRESS)
		velocity[x] -= SCOP_VELOCITY;

	if (glfwGetKey(scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		velocity[y] += SCOP_VELOCITY;
	if (glfwGetKey(scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		velocity[y] -= SCOP_VELOCITY;

	moved = velocity[x] != 0 || velocity[y] != 0 || velocity[z] != 0;
	if (moved != 0)
		camera_move_rel(&scene->cam, velocity);

	return moved;
}

static inline void	scop_draw_array(const vertex_cnt *container, attrib attribute, GLenum mode)
{
	debug("draw_array: Drawing container to %d, vertex_size: %zu\n", attribute, container->vertex_size / sizeof(*container->data));
	glEnableVertexAttribArray(attribute);

	glVertexAttribPointer(
		attribute,	// attribute
		container->vertex_size / sizeof(*container->data), // vertex size
		GL_FLOAT,	// data type
		GL_FALSE,	// normalized
		0,			// stride
		NULL		// array buffer offset
	);


	debug("draw_array: vertex count: %zu\n", container->count);

	glDrawArrays(mode, 0, container->count);

	glDisableVertexAttribArray(attribute);
}

static inline void	scop_draw_obj(scop *scene)
{
	scop_draw_array(&scene->obj.v, ATTRIB_VERTEX, GL_TRIANGLES);
}

int			scop_loop(scop *scene)
{
	mat4	mvp;
	GLuint	vao_id;
	GLuint	vb_id;
	GLint	matrix_id;

	if (scene->obj.v.count == 0)
		return 1;

	vertex_array_object(&vao_id);
	vertex_buffer(&vb_id);

	vertex_buffer_data(vb_id, &scene->obj.v, GL_STATIC_DRAW);

	const vec3	pos = { 0, 3, 5 };
	const vec3	target = { 0, 0, 0 };
	const vec3	up = { 0, 1, 0 };

	camera_lookat(&scene->cam, pos, target, up);

	matrix_id = glGetUniformLocation(scene->shader_id, "MVP");

//		debug("shader id: %u, matrix_id: %u\n", scene->shader_id, matrix_id);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader
		glUseProgram(scene->shader_id);
		// Apply MVP
		camera_project(&scene->cam, mvp, scene->obj.model);

		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		scop_draw_obj(scene);

		// Swap buffers
		glfwSwapBuffers(scene->window);

		do glfwWaitEvents();
		while (scene_movement(scene) == 0);
//		glfwPollEvents();
	} while (glfwGetKey(scene->window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(scene->window) == 0);

	return 0;
}

int			scop_load_obj_raw(scop *scene, const void *vertices, GLsizeiptr size)
{
	return object_load_raw(&scene->obj, vertices, size);
}

int			scop_load_obj_file(scop *scene, const char *file_path)
{
	FILE	*file;
	int		ret;

	file = fopen(file_path, "r");
	ret = -(file == NULL);
	if (ret == 0)
	{
		ret = object_load(&scene->obj, file);
		fclose(file);
	}
	else
		error("fopen: %s: %s\n", file_path, strerror(errno));

	return ret;
}
