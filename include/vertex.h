#pragma once

#include <GL/glew.h>

#include <vertex_cnt.h>

void	vertex_array_object(GLuint *vertex_array_id);

void	vertex_buffer(GLuint *vertex_buffer_id);

void	vertex_buffer_data(GLuint buffer_id, vertex_cnt *container,
	GLenum usage);
