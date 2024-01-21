#pragma once

#include <GL/glew.h>

#include <vertex_cnt.h>
#include <face_cnt.h>

void	vertex_array_init(GLuint *vertex_array_id);

void	vertex_buffer_init(GLuint *vertex_buffer_id);

//void	vertex_index_buffer_init(GLuint *vertex_buffer_id);

void	vertex_buffer_load(GLuint buffer_id, vertex_cnt *container,
	GLenum usage);

GLsizei	vertex_index_buffer_load(GLuint buffer_id, face_cnt *container,
	GLsizeiptr vertex_count, GLenum usage);
