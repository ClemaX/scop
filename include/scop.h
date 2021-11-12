#pragma once

#include <window.h>
#include <camera.h>

#include <object.h>

#ifdef DEBUG
# define SCOP_WINDOW_NAME "scop"
#else
# define SCOP_WINDOW_NAME "scop (debug)"
#endif

#define SHADER_DIR "shaders"

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
	GLFWwindow		*window;
}				scop;


int		scop_init(scop *scene);
void	scop_terminate();

int		scop_loop(scop *scene);

int		scop_load_obj_raw(scop *scene, const void *vertices, GLsizeiptr size);
int		scop_load_obj_file(scop *scene, const char *file_path);
