#include <matrix.h>

#include <vector.h>

#include <string.h>

#include <math.h>

void		matrix_perspective(mat4 dst, float fov, float near, float far)
{
	const GLfloat		scale = 1 / tanf(fov / 2 * (M_PI / 180));

	dst = (mat4){
		{scale,	0,		0,							0},
		{0,		scale,	0,							0},
		{0,		0,		-far / (far - near),		-1},
		{0,		0,		-far * near / (far - near),	0},
	};
}

//void	matrix_orthographic();

void		matrix_mul4_vec(vec4 dst, mat4 matrix, vec4 point)
{
	dst[x] = point[x] * matrix[x][x] + point[y] * matrix[y][x] + point[z] * matrix[z][x];
	dst[y] = point[x] * matrix[x][y] + point[y] * matrix[y][y] + point[z] * matrix[z][y];
	dst[z] = point[x] * matrix[x][z] + point[y] * matrix[y][z] + point[z] * matrix[z][z];
	dst[w] = point[x] * matrix[x][w] + point[y] * matrix[y][w] + point[z] * matrix[z][w];
}

static void	matrix_mul(GLfloat **dst, const GLfloat **a, const GLfloat **b,
	GLuint n, GLuint m, GLuint p)
{
	for (GLuint i = 0; i < p; i++)
	{
		for (GLuint j = 0; j < m; j++)
		{
			dst[i][j] = 0;
			for (GLuint k = 0; k < n; k++)
				dst[i][j] += a[i][k] * b[k][j];
		}
	}
}

void		matrix_mul4(mat4 dst, const mat4 a, const mat4 b)
{
	matrix_mul((GLfloat**)dst, (const GLfloat**)a, (const GLfloat**)b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat4) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void		matrix_mul3(mat3 dst, const mat3 a, const mat3 b)
{
	matrix_mul((GLfloat**)dst, (const GLfloat**)a, (const GLfloat**)b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat3) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void		matrix_mul2(mat2 dst, const mat2 a, const mat2 b)
{
	matrix_mul((GLfloat**)dst, (const GLfloat**)a, (const GLfloat**)b,
		sizeof(*a) / sizeof(**a),
		sizeof(mat2) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void		matrix_identity4(mat4 mat, GLfloat value)
{
	matrix_identity(mat, value);
}
