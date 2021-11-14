#pragma once

#include <window.h>
#include <camera.h>

#include <object.h>

#ifdef DEBUG
# define SCOP_WINDOW_NAME "scop"
#else
# define SCOP_WINDOW_NAME "scop (debug)"
#endif

#ifndef SCOP_VELOCITY
# define SCOP_VELOCITY 0.2f
#endif

#ifndef SHADER_DIR
# define SHADER_DIR "./shaders"
#endif

typedef enum	attrib
{
	ATTRIB_VERTEX = 0,
	ATTRIB_FRAGMENT = 1,
}				attrib;

#define NAME_MVP "MVP"


typedef struct	scop_settings
{
	int			gl_major;
	int			gl_minor;
	int			anti_aliasing;
	int			width;
	int			height;
	const char	*fragment_shader;
	const char	*vertex_shader;
}				scop_settings;

typedef struct	scop
{
	scop_settings	settings;
	camera			cam;
	object			obj;
	GLuint			shader_id;
	GLint			mvp_loc;
	GLuint			vao_id;
	GLuint			vb_id;
	GLFWwindow		*window;
}				scop;

typedef void 	(*scop_resize_cb)(scop *scop, GLuint width, GLuint height);


int		scop_init(scop *scene);
void	scop_terminate();

void	scop_draw(scop *scene);
int		scop_loop(scop *scene);

int		scop_load_obj_raw(scop *scene, const void *vertices, GLsizeiptr size);
int		scop_load_obj_file(scop *scene, const char *file_path);
