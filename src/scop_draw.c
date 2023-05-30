#include "camera.h"
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
	glUseProgram(scene->shader_id);

	// Apply MVP
	camera_project(&scene->cam, mvp, scene->obj.model);

	glUniformMatrix4fv(scene->mvp_loc, 1, GL_FALSE, &mvp[0][0]);
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
	if (scene->shader_id != 0)
	{
		glDeleteShader(scene->shader_id);
		scene->shader_id = 0;
	}
	glfwTerminate();
	object_destroy(&scene->obj);
}

static inline int	scop_movement(scop *scene, GLdouble delta)
{
	const float	distance = delta * SCOP_VELOCITY;
	vec3	velocity = { 0, 0, 0 };
	vec2	rotation = {0, 0};
	int		moved;

	if (scene->input & INPUT_EXIT)
		return -1;

	if (scene->input & INPUT_ZOOM_IN)
		velocity[z] += distance;
	if (scene->input & INPUT_ZOOM_OUT)
		velocity[z] -= distance;

	if (scene->input & INPUT_PAN_UP)
		velocity[y] += distance;
	if (scene->input & INPUT_PAN_DOWN)
		velocity[y] -= distance;
	if (scene->input & INPUT_PAN_LEFT)
		velocity[x] += distance;
	if (scene->input & INPUT_PAN_RIGHT)
		velocity[x] -= distance;

	if (scene->input & INPUT_ROTATE_UP)
		rotation[y] += delta; //distance;
	if (scene->input & INPUT_ROTATE_DOWN)
		rotation[y] -= delta; //distance;

	if (scene->input & INPUT_ROTATE_LEFT)
		rotation[x] += delta; //distance;
	if (scene->input & INPUT_ROTATE_RIGHT)
		rotation[x] -= delta; //distance;

	moved = velocity[x] || velocity[y] || velocity[z] || rotation[x] || rotation[y];

	if (moved != 0)
	{
		vec3 cam_pos;

		camera_rotate(&scene->cam, rotation[x], rotation[y]);
		//camera_move_rel(&scene->cam, velocity);

		camera_to_cartesian(&scene->cam, cam_pos);

		map_point_position_set(&scene->map, POINT_CAMERA, (int[2]){cam_pos[x], cam_pos[z]});

		map_draw(&scene->map);
	}

	return moved;
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
