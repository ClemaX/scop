#include <stdio.h>
#include <string.h>
#include <math.h>

#include <matrix.h>
#include <vector.h>

#include <logger.h>

void		matrix_perspective(mat4 dst, float fov, float near, float far)
{
	const GLfloat	scale = 1.0f / tanf(fov / 2.0f * (M_PI / 180.0f));

	const mat4		projection = {
		{scale,	0,		0,								0},
		{0,		scale,	0,								0},
		{0,		0,		far / (far - near),				1},
		{0,		0,		-(far * near) / (far - near),   0},
	};

	memcpy(dst, projection, sizeof(mat4));
}

void		matrix_mul4_vec(vec4 dst, const mat4 mat, const vec3 point)
{
	dst[x] = point[x] * mat[x][x] + point[y] * mat[y][x] + point[z] * mat[z][x] + point[w] * mat[w][x];
	dst[y] = point[x] * mat[x][y] + point[y] * mat[y][y] + point[z] * mat[z][y] + point[w] * mat[w][y];
	dst[z] = point[x] * mat[x][z] + point[y] * mat[y][z] + point[z] * mat[z][z] + point[w] * mat[w][z];
	dst[w] = point[x] * mat[x][w] + point[y] * mat[y][w] + point[z] * mat[z][w] + point[w] * mat[w][w];
}

static void	matrix_mul(GLfloat *dst, const GLfloat *a, const GLfloat *b,
	GLuint n, GLuint m, GLuint p)
{
	for (GLuint i = 0; i < n; i++)
	{
        for (GLuint j = 0; j < p; j++)
		{
            GLfloat sum = 0.0;

            for (GLuint k = 0; k < m; k++)
			{
                sum += a[i * m + k] * b[k * p + j];
            }

            dst[i * p + j] = sum;
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
