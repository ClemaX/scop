#include <math.h>
#include <vector.h>

static inline GLfloat	vec_mag(const vec3 vec)
{
	return sqrtf(vec[_v_x] * vec[_v_x] + vec[_v_y] * vec[_v_y] + vec[_v_z] * vec[_v_z]);
}

void	vec_cross(vec3 dst, const vec3 a, const vec3 b)
{
	dst[_v_x] = a[_v_y] * b[_v_z] - a[_v_z] * b[_v_y];
	dst[_v_y] = a[_v_z] * b[_v_x] - a[_v_x] * b[_v_z];
	dst[_v_z] = a[_v_x] * b[_v_y] - a[_v_y] * b[_v_x];
}

float	vec_dot(const vec3 a, const vec3 b)
{
    return a[_v_x] * b[_v_x] + a[_v_y] * b[_v_y] + a[_v_z] * b[_v_z];
}

void	vec_normalize(vec3 dst, const vec3 src)
{
	const GLfloat mag = vec_mag(src);

	dst[_v_x] = src[_v_x] / mag;
	dst[_v_y] = src[_v_y] / mag;
	dst[_v_z] = src[_v_z] / mag;
}
