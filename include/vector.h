#pragma once

#include <GL/glew.h>

#define _v_x 0
#define _v_y 1
#define _v_z 2
#define _v_w 3

typedef GLboolean	bvec2[2];
typedef GLint		ivec2[2];
typedef GLuint		uvec2[2];
typedef GLfloat		vec2[2];
typedef GLdouble	dvec2[2];

typedef GLboolean	bvec3[2];
typedef GLint		ivec3[3];
typedef GLuint		uvec3[3];
typedef GLfloat		vec3[3];
typedef GLdouble	dvec3[3];

typedef GLboolean	bvec4[2];
typedef GLint		ivec4[4];
typedef GLuint		uvec4[4];
typedef GLfloat		vec4[4];
typedef GLdouble	dvec4[4];

void	vec_cross(vec3 dst, const vec3 a, const vec3 b);
float	vec_dot(const vec3 a, const vec3 b);
void	vec_normalize(vec3 dst, const vec3 src);
