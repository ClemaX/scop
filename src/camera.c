#include <camera.h>

void	camera_project(const camera *cam, mat4 mvp, const mat4 model)
{
	matrix_mul4(mvp, cam->projection, cam->view);
	matrix_mul4(mvp, mvp, model);
}
