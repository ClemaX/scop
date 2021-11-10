#include <matrix.h>

#include <vector.h>

#include <string.h>

#include <math.h>

#ifndef M_PI
# define M_PI 3.141592741f
#endif

void matrix_perspective(mat4 dst, float fov, float aspect_ratio, float near, float far)
{
	static const mat4	projection_matrix = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, -1, -1},
		{0, 0, 0, 0},
	};

	const GLfloat		scale = 1 / tanf(fov / 2 * (M_PI / 180));

	dst = (mat4){
		{scale,	0,		0,							0},
		{0,		scale,	0,							0},
		{0,		0,		-far / (far - near),		-1},
		{0,		0,		-far * near / (far - near),	0},
	};
}

//void	matrix_orthographic();

void	matrix_mul4_vec(vec4 dst, mat4 matrix, vec4 point)
{
	dst[x] = dst[x] * matrix[x][x] + dst[y] * matrix[y][x] + dst[z] * matrix[z][x];
	dst[y] = dst[x] * matrix[x][y] + dst[y] * matrix[y][y] + dst[z] * matrix[z][y];
	dst[z] = dst[x] * matrix[x][z] + dst[y] * matrix[y][z] + dst[z] * matrix[z][z];
	dst[w] = dst[x] * matrix[x][w] + dst[y] * matrix[y][w] + dst[z] * matrix[z][w];
}

static void	matrix_mul(GLfloat **dst, const GLfloat **a, const GLfloat **b, GLuint n, GLuint m, GLuint p)
{
	for (int i = 0; i < p; i++)
	{
		for (int j = 0; j < m; j++)
		{
			dst[i][j] = 0;
			for (int k = 0; k < n; k++)
				dst[i][j] += a[i][k] * b[k][j];
		}
	}
}

void	matrix_mul4(mat4 dst, const mat4 a, const mat4 b)
{
	matrix_mul(dst, a, b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat4) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void	matrix_mul3(mat3 dst, const mat3 a, const mat3 b)
{
	matrix_mul(dst, a, b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat3) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void	matrix_mul2(mat2 dst, const mat2 a, const mat2 b)
{
	matrix_mul(dst, a, b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat2) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}
