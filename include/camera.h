#pragma once

#include <matrix.h>

typedef struct	camera
{
	mat4	projection;
	mat4	view;
}				camera;

void	camera_project(const camera *cam, mat4 mvp, const mat4 model);
