#pragma once

#include <vector.h>

typedef vec2 	mat2x2[2];
typedef vec2 	mat2x3[3];
typedef vec2 	mat2x4[4];
typedef mat2x2	mat2;

typedef vec3 	mat3x2[2];
typedef vec3 	mat3x3[3];
typedef vec3 	mat3x4[4];
typedef mat3x3	mat3;

typedef vec4 	mat4x2[2];
typedef vec4 	mat4x3[3];
typedef vec4 	mat4x4[4];
typedef mat4x4	mat4;

typedef dvec2 	dmat2x2[2];
typedef dvec2 	dmat2x3[3];
typedef dvec2 	dmat2x4[4];
typedef dmat2x2	dmat2;

typedef dvec3 	dmat3x2[2];
typedef dvec3 	dmat3x3[3];
typedef dvec3 	dmat3x4[4];
typedef dmat3x3	dmat3;

typedef dvec4 	dmat4x2[2];
typedef dvec4 	dmat4x3[3];
typedef dvec4 	dmat4x4[4];
typedef dmat4x4	dmat4;

void	matrix_perspective(mat4 dst, float fov, float aspect_ratio, float near, float far);
void	matrix_mul_point(mat4 matrix, vec3 *point);
