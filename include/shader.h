#pragma once

#include <GL/glew.h>

typedef struct	s_shader
{
	GLuint	id;
	GLint	mvp_loc;
	GLint	res_loc;
}				shader;

GLuint	shader_load(const char *vertex_file, const char *fragment_file);
