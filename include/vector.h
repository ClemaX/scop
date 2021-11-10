#pragma once

#include <GL/gl.h>

#define declare_vec2(type) union { \
	GLint	data[2]; \
	struct \
	{ \
		GLint	x; \
		GLint	y; \
	}; \
}

#define declare_vec3(type) union { \
	GLint	data[3]; \
	struct \
	{ \
		GLint	x; \
		GLint	y; \
		GLint	z; \
	}; \
}

#define declare_vec4(type) union { \
	GLint	data[4]; \
	struct \
	{ \
		GLint	x; \
		GLint	y; \
		GLint	z; \
		GLint	w; \
	}; \
}

typedef declare_vec2(GLboolean)	bvec2;
typedef declare_vec2(GLint)		ivec2;
typedef declare_vec2(GLuint)	uvec2;
typedef declare_vec2(GLfloat)	vec2;
typedef declare_vec2(GLdouble)	dvec2;

typedef declare_vec2(GLboolean)	bvec3;
typedef declare_vec3(GLint)		ivec3;
typedef declare_vec3(GLuint)	uvec3;
typedef declare_vec3(GLfloat)	vec3;
typedef declare_vec3(GLdouble)	dvec3;

typedef declare_vec2(GLboolean)	bvec4;
typedef declare_vec4(GLint)		ivec4;
typedef declare_vec4(GLuint)	uvec4;
typedef declare_vec4(GLfloat)	vec4;
typedef declare_vec4(GLdouble)	dvec4;
