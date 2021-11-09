#pragma once

#include <window.h>

#ifdef DEBUG
# define SCOP_WINDOW_NAME "scop"
#else
# define SCOP_WINDOW_NAME "scop (debug)"
#endif

#define SHADER_DIR "shaders"

typedef struct	scop_settings
{
	int	gl_major;
	int	gl_minor;
	int	anti_aliasing;
	int	width;
	int	height;
}				scop_settings;

GLFWwindow	*scop_init(scop_settings *settings, GLuint *shader_program_id);
int			scop_loop(GLFWwindow *window, GLuint shader_program_id);
void		scop_terminate();
