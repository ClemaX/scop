#include <string.h>
#include <math.h>

#include <camera.h>

#include <logger.h>

static inline void	camera_position_get(const camera *cam, vec3 pos)
{
	pos[_v_x] = cam->target[_v_x] + cam->distance * sinf(cam->phi) * sinf(cam->theta);
	pos[_v_y] = cam->target[_v_y] + cam->distance * cosf(cam->phi);
	pos[_v_z] = cam->target[_v_z] + cam->distance * sinf(cam->phi) * cosf(cam->theta);
}

static inline void	camera_front_get(const camera *cam, vec3 front)
{
	front[_v_x] = sinf(cam->phi) * sinf(cam->theta);
	front[_v_y] = cosf(cam->phi);
	front[_v_z] = sinf(cam->phi) * cosf(cam->theta);

    vec_normalize(front, front);
}

static inline void	camera_view_update(camera *cam)
{
    vec4 pos;
    vec3 front;
    vec3 side;

    camera_position_get(cam, pos);

    debug_vec3("pos", pos);

    front[_v_x] = cam->target[_v_x] - pos[_v_x];
    front[_v_y] = cam->target[_v_y] - pos[_v_y];
    front[_v_z] = cam->target[_v_z] - pos[_v_z];

    vec_normalize(front, front);

	// Calculate side vector
    vec_cross(side, front, cam->up);
    vec_normalize(side, side);

    // Calculate the corrected up vector
    vec_cross(cam->up, side, front);
    vec_normalize(cam->up, cam->up);

	// Calculate view transformation matrix
  	mat4 view = {
        {side[_v_x],			side[_v_y],				side[_v_z],				0},
        {cam->up[_v_x],			cam->up[_v_y],			cam->up[_v_z],			0},
        {-front[_v_x],			front[_v_y],			-front[_v_z],			0},
        {-vec_dot(pos, side),	-vec_dot(pos, cam->up),	vec_dot(pos, front),	1},
    };

    memcpy(cam->view, view, sizeof(mat4));

	cam->view_dirty = false;
}


void	camera_init(camera *cam, float fov, float near, float far)
{
	matrix_perspective(cam->projection, fov, near, far);

	bzero(cam->target, sizeof(cam->target));
	bzero(cam->up, sizeof(cam->up));

	cam->up[_v_y] = 1;

	cam->phi = M_PI / 2;
	cam->theta = 0;

	cam->distance = 10.0f;

	cam->view_dirty = true;
}

/*
void	camera_move(camera *cam, const vec3 new_pos)
{
	memcpy(cam->pos, new_pos, sizeof(cam->pos));
	camera_view(cam);
} */

void	camera_move_rel(camera *cam, const vec3 vel)
{
	vec3	front;
	vec3	side;

	camera_front_get(cam, front);

	vec_cross(side, front, cam->up);
	vec_normalize(side, side);

	cam->target[_v_x] += (front[_v_x] + side[_v_x] + cam->up[_v_x]) * vel[_v_x];
	cam->target[_v_y] += (front[_v_y] + side[_v_y] + cam->up[_v_y]) * vel[_v_y];
	cam->distance -= vel[_v_z];

	debug_vec3("target", cam->target);

	cam->view_dirty = true;
}

void	camera_rotate(camera *cam, GLfloat d_theta, GLfloat d_phi)
{
	cam->theta += d_theta;
	cam->phi += d_phi;

	/* if (cam->theta >= M_PI) {
		cam->theta -= M_PI;
	}
	else if (cam->theta <= -M_PI) {
		cam->theta += M_PI;
	}

	if (cam->phi >= M_PI) {
		cam->phi -= M_PI;
	}
	else if (cam->phi <= -M_PI) {
		cam->phi += M_PI;
	} */

/*
	(void)(d_phi);

	if (cam->up[_v_y] > 0)
		cam->theta += d_theta;
	else
		cam->theta -= d_theta;

	if (cam->phi > M_PI * 2.0f)
		cam->phi -= M_PI * 2.0f;
	else if (cam->phi < -M_PI * 2.0f)
		cam->phi += M_PI * 2.0f; */

/* 	if ((cam->phi > 0 && cam->phi < M_PI)
		|| (cam->phi < -M_PI && cam->phi > -M_PI * 2.0f))
		cam->up[_v_y] = 1;
	else
		cam->up[_v_y] = -1;
 */

	debug("theta: %f\n", cam->theta);
	debug("phi: %f\n", cam->phi);

	cam->view_dirty = true;
}

void	camera_target(camera *cam, const vec3 new_target)
{
	memcpy(cam->target, new_target, sizeof(cam->target));

	cam->view_dirty = true;
}

void	camera_lookat(camera *cam, const vec3 up, const vec3 target, GLfloat distance)
{
	cam->distance = distance;

	memcpy(cam->up, up, sizeof(cam->up));
	memcpy(cam->target, target, sizeof(cam->target));

	cam->view_dirty = true;
}

void	camera_project(camera *cam, mat4 mvp, const mat4 model)
{
	if (cam->view_dirty)
		camera_view_update(cam);

	matrix_mul4(mvp, cam->view, cam->projection);
	matrix_mul4(mvp, mvp, model);

	debug("mvp:\n", stderr);
	matrix_print(&mvp[0][0], 4, 4, stderr);
}
