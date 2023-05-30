#pragma once

#include <matrix.h>

/*
typedef enum	camera_mode
{
	CAM_PERSPECTIVE,
	CAM_ORTHOGRAPHIC,
}				camera_mode;
 */

typedef struct	camera
{
	mat4	projection;
	mat4	view;
	vec3	up;
	vec3	target;
	GLfloat	distance;
	GLfloat	theta;
	GLfloat	phi;
}				camera;


void	camera_init(camera *cam, float fov, float near, float far);

void	camera_move(camera *cam, const vec3 new_pos);
void	camera_move_rel(camera *cam, const vec3 velocity);
void	camera_target(camera *cam, const vec3 new_target);
void	camera_lookat(camera *cam, const vec3 up,
	const vec3 target, GLfloat distance);
void	camera_rotate(camera *cam, GLfloat d_theta, GLfloat d_phi);

void	camera_to_cartesian(const camera *cam, vec3 pos);

void	camera_project(const camera *cam, mat4 mvp, const mat4 model);
