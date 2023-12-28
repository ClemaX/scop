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
		{0,		0,		(near + far) / (near - far),	-1},
		{0,		0,		2 * near * far / (near - far),   0},
	};

	memcpy(dst, projection, sizeof(mat4));
}

void		matrix_mul4_vec(vec4 dst, const mat4 mat, const vec3 point)
{
	dst[_v_x] = point[_v_x] * mat[_v_x][_v_x] + point[_v_y] * mat[_v_y][_v_x] + point[_v_z] * mat[_v_z][_v_x] + point[_v_w] * mat[_v_w][_v_x];
	dst[_v_y] = point[_v_x] * mat[_v_x][_v_y] + point[_v_y] * mat[_v_y][_v_y] + point[_v_z] * mat[_v_z][_v_y] + point[_v_w] * mat[_v_w][_v_y];
	dst[_v_z] = point[_v_x] * mat[_v_x][_v_z] + point[_v_y] * mat[_v_y][_v_z] + point[_v_z] * mat[_v_z][_v_z] + point[_v_w] * mat[_v_w][_v_z];
	dst[_v_w] = point[_v_x] * mat[_v_x][_v_w] + point[_v_y] * mat[_v_y][_v_w] + point[_v_z] * mat[_v_z][_v_w] + point[_v_w] * mat[_v_w][_v_w];
}

static void	matrix_mul(GLfloat *dst, const GLfloat *a, const GLfloat *b,
	GLuint n, GLuint m, GLuint p)
{
	for (GLuint i = 0; i < n; i++)
	{
        for (GLuint j = 0; j < p; j++)
		{
            GLfloat sum = 0;

            for (GLuint k = 0; k < m; k++)
                sum += a[i * m + k] * b[k * p + j];

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
	fprintf(file, "{\n");

	for (GLuint i = 0; i < m; i++)
	{
		fprintf(file, "\t{ ");
		fprintf(file, "%f", mat[i * n + 0]);
		for (GLuint j = 1; j < n; j++)
		{
			fprintf(file, ", %f", mat[i * n + j]);
		}
		fprintf(file, " }\n");
	}
	fprintf(file, "}\n");
}
