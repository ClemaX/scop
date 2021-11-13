#include <GL/glew.h>
#include <string.h>
#include <errno.h>

#include <scop.h>
#include <vertex.h>
#include <shader.h>

#include <logger.h>

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

int			scop_loop(scop *scene)
{
	GLuint	vao;
	GLuint	vb;

	if (scene->obj.v.count == 0)
		return 1;

	vertex_array_object(&vao);
	vertex_buffer(&vb);

	vertex_buffer_data(&scene->obj.v, GL_STATIC_DRAW);

	const vec3	pos = {1, 5, 1};
	const vec3	target = {0, 0, 0};
	const vec3	up = {0, 1, 0};

	camera_lookat(&scene->cam, pos, target, up);

	mat4	mvp;


	GLint	matrix_id = glGetUniformLocation(scene->shader_id, "MVP");

//		debug("shader id: %u, matrix_id: %u\n", scene->shader_id, matrix_id);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader
		glUseProgram(scene->shader_id);
		// Apply MVP
		camera_project(&scene->cam, mvp, scene->obj.model);
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
//		gl_perror();

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			0,			// attribute
			4,			// vertex size
			GL_FLOAT,	// data type
			GL_FALSE,	// normalized
			0,			// stride
			(void*)0	// array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

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
