#include <math.h>
#include <vector.h>

static inline GLfloat	vec_mag(const vec3 vec)
{
	return sqrtf(vec[x] * vec[x] + vec[y] * vec[y] + vec[z] * vec[z]);
}

void	vec_cross(vec3 dst, const vec3 a, const vec3 b)
{
	dst[x] = a[y] * b[z] - a[z] * b[y];
	dst[y] = -a[x] * b[z] - a[z] * b[x];
	dst[z] = a[x] * b[y] - a[y] * b[x];
}

float vec_dot(const vec3 a, const vec3 b)
{
    return a[x] * b[x] + a[y] * b[y] + a[z] * b[z];
}

void	vec_normalize(vec3 dst, const vec3 src)
{
	const GLfloat mag = vec_mag(src);

	dst[x] = src[x] / mag;
	dst[y] = src[y] / mag;
	dst[z] = src[z] / mag;
}
