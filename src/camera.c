#include <string.h>
#include <math.h>

#include <camera.h>

#include <logger.h>

void				camera_to_cartesian(const camera *cam, vec3 pos)
{
	pos[x] = cam->target[x] + cam->distance * sinf(cam->phi) * sinf(cam->theta);
	pos[y] = cam->target[y] + cam->distance * cosf(cam->phi);
	pos[z] = cam->target[z] + cam->distance * sinf(cam->phi) * cosf(cam->theta);
}

static inline void camera_view(camera *cam)
{
    vec4 pos;
    vec3 front;
    vec3 side;

    camera_to_cartesian(cam, pos);

    debug_vec3("pos", pos);

    front[x] = cam->target[x] - pos[x];
    front[y] = cam->target[y] - pos[y];
    front[z] = cam->target[z] - pos[z];

    vec_normalize(front, front);

    vec_cross(side, front, cam->up);
    vec_normalize(side, side);

    // Calculate the corrected up vector
    //vec_cross(cam->up, side, front);
    //vec_normalize(cam->up, cam->up);

    debug_vec3("target", cam->target);
    debug_vec3("front", front);
    debug_vec3("up", cam->up);
    debug_vec3("side", side);

    mat4 view = {
        {side[x],		side[y],	side[z],	-vec_dot(pos, side)},
        {cam->up[x],	cam->up[y],	cam->up[z],	-vec_dot(pos, cam->up)},
        {-front[x],		-front[y],	-front[z],	vec_dot(pos, front)},
        {0,				0,			0,			1},
    };

    memcpy(cam->view, view, sizeof(mat4));
}


void	camera_init(camera *cam, float fov, float near, float far)
{
	matrix_perspective(cam->projection, fov, near, far);

	bzero(cam->target, sizeof(cam->target));
	bzero(cam->up, sizeof(cam->up));

	cam->up[y] = 1;

	cam->phi = M_PI / 2;
	cam->theta = 0;

	cam->distance = 10.0f;

	camera_view(cam);
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

	camera_to_cartesian(cam, front);
	vec_normalize(front, front);

	vec_cross(side, front, cam->up);
	vec_normalize(side, side);

	cam->target[x] += (front[x] - side[x] + cam->up[x]) * vel[x];
	cam->target[y] += (front[y] - side[y] + cam->up[y]) * vel[y];
	cam->distance -= vel[z];

	debug_vec3("target", cam->target);

	camera_view(cam);
}

void	camera_rotate(camera *cam, GLfloat d_theta, GLfloat d_phi)
{
	cam->theta += d_theta;
	/*
	if (cam->up[y] > 0)
		cam->theta += d_theta;
	else
		cam->theta -= d_theta;
	*/

	cam->phi += d_phi;
/*
	if (cam->phi > M_PI * 2.0f)
		cam->phi -= M_PI * 2.0f;
	else if (cam->phi < -M_PI * 2.0f)
		cam->phi += M_PI * 2.0f;

	if ((cam->phi > 0 && cam->phi < M_PI)
		|| (cam->phi < -M_PI && cam->phi > -M_PI * 2.0f))
		cam->up[y] = 1;
	else
		cam->up[y] = -1; */

	debug("theta: %f\n", cam->theta);
	debug("phi: %f\n", cam->phi);


	camera_view(cam);
}

void	camera_target(camera *cam, const vec3 new_target)
{
	memcpy(cam->target, new_target, sizeof(cam->target));

	camera_view(cam);
}

void	camera_lookat(camera *cam, const vec3 up, const vec3 target, GLfloat distance)
{
	cam->distance = distance;

	memcpy(cam->up, up, sizeof(cam->up));
	memcpy(cam->target, target, sizeof(cam->target));

	camera_view(cam);
}

void	camera_project(const camera *cam, mat4 mvp, const mat4 model)
{
	mat4	tmp;

	matrix_mul4(tmp, cam->projection, cam->view);
	matrix_mul4(mvp, tmp, model);
}
