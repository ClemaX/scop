#include <scop.h>

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

static inline int	scop_movement(scop *scene)
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
	int	movement;

	do
	{
		scop_draw(scene);

		do
		{
			glfwWaitEvents();
			movement = scop_movement(scene);
		}
		while (movement == 0);

	} while (movement != -1);

	return 0;
}
