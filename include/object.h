#pragma once

#include <GL/glew.h>

#include <stdio.h>

#include <matrix.h>

#include <object.h>
#include <vertex_cnt.h>
#include <face_cnt.h>

typedef struct	object
{
	vertex_cnt	v;
	vertex_cnt	vt;
	vertex_cnt	vn;
	face_cnt	f;
	mat4		model;
	GLuint		shader_id;
}				object;

typedef int	(*object_cmd_fun)(object *, const char *);

typedef struct	object_cmd
{
	const char		*name;
	const char		*spec;
	object_cmd_fun	exec;
}				object_cmd;

void	object_init(object *object);
void	object_destroy(object *object);

int		object_load(object *object, FILE *file);
int		object_load_raw(object *object, const void *data, GLsizeiptr size);

int		object_write(object *object, FILE *file);

int		object_exec(object *object, char *command);

int		object_v(object *object, const char *arguments);
int		object_vn(object *object, const char *arguments);
int		object_vt(object *object, const char *arguments);
int		object_f(object *object, const char *arguments);
