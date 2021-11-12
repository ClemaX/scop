#pragma once

#include <window.h>
#include <camera.h>

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
	GLuint			shader_id;
	GLFWwindow		*window;
}				scop;


int		scop_init(scop *scene);
int		scop_loop(scop *scene);
void	scop_terminate();
