#include <string.h>
#include <math.h>

#include <camera.h>

#include <logger.h>

static inline void	camera_view(camera *cam)
{
	// normalized direction vector
	vec3		front;

	vec_normal(front,
		cam->target[x] - cam->pos[x],
		cam->target[y] - cam->pos[y],
		cam->target[z] - cam->pos[z]);

//	debug_vec3("pos", cam->pos);
//	debug_vec3("target", cam->target);
//	debug_vec3("front", front);
//	debug_vec3("up", cam->up);

	vec3			side;

	vec_cross(side, front, cam->up);

	vec_normalize(side, side);

//	debug_vec3("side", side);

	vec3			u;

	vec_cross(u, side, front);

 	const mat4		M = {
		{ side[x],	u[x],	-front[x],	-cam->pos[x] },
		{ side[y],	u[y],	-front[y],	-cam->pos[y] },
		{ side[z],	u[z],	-front[z],	-cam->pos[z] },
		{ 0,		0,		0,			1 },
	};

	memcpy(cam->view, M, sizeof(cam->view));
}

void	camera_init(camera *cam, float fov, float near, float far)
{
	matrix_perspective(cam->projection, fov, near, far);

	bzero(cam->pos, sizeof(cam->pos));
	bzero(cam->target, sizeof(cam->target));
	bzero(cam->up, sizeof(cam->up));

	cam->up[y] = 1;

	camera_view(cam);
}

void	camera_move(camera *cam, const vec3 new_pos)
{
	memcpy(cam->pos, new_pos, sizeof(cam->pos));
	camera_view(cam);
}

void	camera_move_rel(camera *cam, const vec3 vel)
{
	vec3	front;
	vec3	side;

	vec_normal(front,
		cam->target[x] - cam->pos[x],
		cam->target[y] - cam->pos[y],
		cam->target[z] - cam->pos[z]);

	vec_cross(side, front, cam->up);
	vec_normalize(side, side);

	cam->pos[x] += (front[x] - side[x] + cam->up[x]) * vel[x];
	cam->pos[y] += (front[y] - side[y] + cam->up[y]) * vel[y];
	cam->pos[z] += (front[z] - side[z] + cam->up[z]) * vel[z];

	camera_view(cam);
}

void	camera_target(camera *cam, const vec3 new_target)
{
	memcpy(cam->target, new_target, sizeof(cam->target));
	camera_view(cam);
}

void	camera_lookat(camera *cam, const vec3 pos, const vec3 target,
	const vec3 up)
{
	memcpy(cam->pos, pos, sizeof(cam->pos));
	memcpy(cam->target, target, sizeof(cam->target));
	memcpy(cam->up, up, sizeof(cam->up));
	camera_view(cam);
}

void	camera_project(const camera *cam, mat4 mvp, const mat4 model)
{
	mat4	tmp;

	matrix_mul4(tmp, cam->projection, cam->view);
	matrix_mul4(mvp, tmp, model);
}
