#pragma once

#include <face.h>

typedef struct	face_cnt
{
	face		*data;
	GLsizei		size;
	GLsizei		count;
}				face_cnt;

int		face_cnt_init(face_cnt *container, GLsizeiptr size);
void	face_cnt_destroy(face_cnt *container);

int		face_cnt_resize(face_cnt *container, GLsizeiptr new_size);

int		face_cnt_push(face_cnt *container, const face *face);

GLuint	*face_cnt_triangulate(face_cnt *container, GLsizeiptr vertex_count, GLsizei *index_count);
