#include <stdio.h>
#include <string.h>
#include <math.h>

#include <matrix.h>
#include <vector.h>

#include <logger.h>

void		matrix_perspective(mat4 dst, float fov, float near, float far)
{
	const GLfloat		scale = 1.0f / tanf(fov / 2.0f * (M_PI / 180.0f));

	const mat4 mat = {
		{scale,	0,		0,								0},
		{0,		scale,	0,								0},
		{0,		0,		far / (far - near),				1},
		{0,		0,		-(far * near) / (far - near),	0},
	};

	memcpy(dst, mat, sizeof(mat4));
}

//void	matrix_orthographic();

void		matrix_mul4_vec(vec4 dst, mat4 mat, vec4 point)
{
	dst[x] = point[x] * mat[x][x] + point[y] * mat[y][x] + point[z] * mat[z][x];
	dst[y] = point[x] * mat[x][y] + point[y] * mat[y][y] + point[z] * mat[z][y];
	dst[z] = point[x] * mat[x][z] + point[y] * mat[y][z] + point[z] * mat[z][z];
	dst[w] = point[x] * mat[x][w] + point[y] * mat[y][w] + point[z] * mat[z][w];
}

static void	matrix_mul(GLfloat *dst, const GLfloat *a, const GLfloat *b,
	GLuint n, GLuint m, GLuint p)
{
	for (GLuint i = 0; i < m; i++)
	{
		for (GLuint j = 0; j < n; j++)
		{
		//	debug("dst[%u][%u] = ", i, j);
			dst[i * n + j] = 0;
			for (GLuint k = 0; k < p; k++)
			{
		//		debug("a[%u][%u] * b[%u][%u] + ", i, k, k, j);
				dst[i * m + j] += a[i * m + k] * b[k * m + j];
			}
		//	debug(" = %f\n", dst[i * m + j]);
		}
	}
}

void		matrix_mul4(mat4 dst, const mat4 a, const mat4 b)
{
	matrix_mul(&dst[0][0], &a[0][0], &b[0][0],
		sizeof(*a) / sizeof(**a),
		sizeof(mat4) / sizeof(*a),
		sizeof(*b) / sizeof(**b));
}

void		matrix_identity4(mat4 mat, GLfloat value)
{
	for (GLuint i = 0; i < 4; i++)
	{
		mat[i][i] = value;
	}
}

void		matrix_print(const GLfloat *mat, GLuint n, GLuint m, FILE *file)
{
	fprintf(file, "{ ");

	for (GLuint i = 0; i < m; i++)
	{
		fprintf(file, "{ ");
		fprintf(file, "%f", mat[i * n + 0]);
		for (GLuint j = 1; j < n; j++)
		{
			fprintf(file, ", %f", mat[i * n + j]);
		}
		fprintf(file, " }");
	}
	fprintf(file, " }\n");
}
