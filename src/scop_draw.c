#include <vector.h>
#include <scop.h>

#include <string.h>
#include <time.h>

#include <logger.h>

void	scop_draw(scop *scene)
{
	mat4	mvp;

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use shader
	glUseProgram(scene->shader.id);

	// Apply MVP
	camera_project(&scene->cam, mvp, scene->obj.model);

	glUniformMatrix4fv(scene->shader.mvp_loc, 1, GL_FALSE, &mvp[0][0]);
	glUniform2f(scene->shader.res_loc, scene->settings.width, scene->settings.height);
//		gl_perror();

	glEnableVertexAttribArray(ATTRIB_VERTEX);

	glVertexAttribPointer(
		ATTRIB_VERTEX,	// attribute
		4,				// vertex size
		GL_FLOAT,		// data type
		GL_FALSE,		// normalized
		0,				// stride
		(void*)0		// array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, scene->obj.v.count);

	glDisableVertexAttribArray(ATTRIB_VERTEX);

	// Swap buffers
	glfwSwapBuffers(scene->window);
}

void		scop_terminate(scop *scene)
{
	if (scene->shader.id != 0)
	{
		glDeleteShader(scene->shader.id);
		scene->shader.id = 0;
	}
	glfwTerminate();
	object_destroy(&scene->obj);
}

static inline int	scop_movement(scop *scene, GLdouble delta)
{
	const float	distance = delta * SCOP_VELOCITY;
	vec3		velocity = {0, 0, 0};
	vec2		rotation = {0, 0};
	bool		moved;
	bool		rotated;

	if (scene->input & INPUT_EXIT)
		return -1;

	if (scene->input & INPUT_ZOOM_IN)
		velocity[_v_z] += distance;
	if (scene->input & INPUT_ZOOM_OUT)
		velocity[_v_z] -= distance;

	if (scene->input & INPUT_PAN_UP)
		velocity[_v_y] += distance;
	if (scene->input & INPUT_PAN_DOWN)
		velocity[_v_y] -= distance;
	if (scene->input & INPUT_PAN_LEFT)
		velocity[_v_x] += distance;
	if (scene->input & INPUT_PAN_RIGHT)
		velocity[_v_x] -= distance;

	if (scene->input & INPUT_ROTATE_UP)
		rotation[_v_y] += distance;
	if (scene->input & INPUT_ROTATE_DOWN)
		rotation[_v_y] -= distance;

	if (scene->input & INPUT_ROTATE_LEFT)
		rotation[_v_x] += distance;
	if (scene->input & INPUT_ROTATE_RIGHT)
		rotation[_v_x] -= distance;

	moved = velocity[_v_x] || velocity[_v_y] || velocity[_v_z];
	rotated = rotation[_v_x] || rotation[_v_y];

	if (moved)
		camera_move_rel(&scene->cam, velocity);
	if (rotated)
		camera_rotate(&scene->cam, rotation[_v_x], rotation[_v_y]);

	return moved || rotated;
}

static inline int	keymap_get(const scop_keymap keymap, int key)
{
	unsigned	i;

	for (i = 0; i < sizeof(scop_keymap) / sizeof(*keymap) && keymap[i] != key;
		i++);

	if (i == sizeof(scop_keymap) / sizeof(*keymap))
		return -1;
	return i;
}

static void	key_callback(GLFWwindow *window, int key, int scancode, int action,
	int mods)
{
	scop	*scene;
	int		scop_key;

	(void)mods;
	(void)scancode;
	scene = glfwGetWindowUserPointer(window);

	if (scene != NULL && (action == GLFW_RELEASE || action == GLFW_PRESS))
	{
		scop_key = keymap_get(scene->settings.keymap, key);
		if (scop_key != -1)
		{
			/* debug("'%s' key state: %d, mods: %d, scancode: %d\n",
				scop_keystrtab[scop_key], action, mods, scancode); */
			if (action == GLFW_RELEASE)
				scene->input &= ~(1 << scop_key);
			else if (action == GLFW_PRESS)
				scene->input |= (1 << scop_key);
		}
	}
}

int			scop_loop(scop *scene)
{
	int			movement;
	GLdouble	delta;

	glfwSetKeyCallback(scene->window, &key_callback);

	do
	{
		// Reset timer
		glfwSetTime(0);

		// Draw scene
		scop_draw(scene);

		// Store time delta
		delta = glfwGetTime();

		movement = 0;
		do
		{
			glfwPollEvents();

			if (scene->input != 0)
				movement = scop_movement(scene, delta);
		}
		while (movement == 0);
	} while (movement != -1);

	glfwSetKeyCallback(scene->window, NULL);

	return 0;
}
