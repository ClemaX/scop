#pragma once

#include <face.h>

typedef struct	face_cnt
{
	face		*data;
	GLsizeiptr	size;
	GLsizeiptr	count;
}				face_cnt;

int		face_cnt_init(face_cnt *container, GLsizeiptr size);
void	face_cnt_destroy(face_cnt *container);

int		face_cnt_resize(face_cnt *container, GLsizeiptr new_size);

int		face_cnt_push(face_cnt *container, const face *face);
