#pragma once

#include <GL/glew.h>

typedef struct	vertex_cnt
{
	GLfloat		*data;
	GLsizeiptr	vertex_size;
	GLsizeiptr	size;
	GLsizeiptr	count;
}				vertex_cnt;

int		vertex_cnt_init(vertex_cnt *container, const void *data,
	GLsizeiptr vertex_size, GLsizeiptr size);
void	vertex_cnt_destroy(vertex_cnt *container);

int		vertex_cnt_resize(vertex_cnt *container, GLsizeiptr size);

int		vertex_cnt_push(vertex_cnt *container, const void *vec);
int		vertex_cnt_cat(vertex_cnt *container, const vertex_cnt *other);
