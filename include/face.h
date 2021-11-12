#pragma once

#include <GL/glew.h>

typedef struct	face_elem
{
	GLint	v;
	GLint	vt;
	GLint	vn;
}				face_elem;

typedef struct	face
{
	face_elem	*elems;
	GLsizeiptr	count;
}				face;

void	face_destroy(face *face);
