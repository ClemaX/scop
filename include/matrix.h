#pragma once

#include <stdio.h>

#include <vector.h>

#ifndef M_PI
# define M_PI 3.141592741f
#endif

#define matrix_identity(mat, value)\
	for (GLuint i = 0; i < sizeof(*mat) / sizeof(**mat); i++)\
		mat[i][i] = value;

typedef vec2 	mat2x2[2];
typedef vec3 	mat2x3[2];
typedef vec4 	mat2x4[2];
typedef mat2x2	mat2;

typedef vec2 	mat3x2[3];
typedef vec3 	mat3x3[3];
typedef vec4 	mat3x4[3];
typedef mat3x3	mat3;

typedef vec2 	mat4x2[4];
typedef vec3 	mat4x3[4];
typedef vec4 	mat4x4[4];
typedef mat4x4	mat4;

typedef dvec2 	dmat2x2[2];
typedef dvec3 	dmat2x3[2];
typedef dvec4 	dmat2x4[2];
typedef dmat2x2	dmat2;

typedef dvec2 	dmat3x2[3];
typedef dvec3 	dmat3x3[3];
typedef dvec4 	dmat3x4[3];
typedef dmat3x3	dmat3;

typedef dvec2 	dmat4x2[4];
typedef dvec3 	dmat4x3[4];
typedef dvec4 	dmat4x4[4];
typedef dmat4x4	dmat4;

void	matrix_perspective(mat4 dst, float fov, float near, float far);

void	matrix_mul4(mat4 dst, const mat4 a, const mat4 b);
//void	matrix_mul3(mat3 *dst, const mat3 *a, const mat3 *b);
//void	matrix_mul2(mat2 *dst, const mat2 *a, const mat2 *b);

void	matrix_identity4(mat4 mat, GLfloat value);
void	matrix_print(const GLfloat *mat, GLuint n, GLuint m, FILE *file);
