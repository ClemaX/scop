#pragma once

#include "map.h"
#include <window.h>
#include <camera.h>

#include <object.h>

#ifdef DEBUG
# define SCOP_WINDOW_NAME "scop"
#else
# define SCOP_WINDOW_NAME "scop (debug)"
#endif

#ifndef SCOP_VELOCITY
# define SCOP_VELOCITY 1.0f
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

#define INPUT_ZOOM_IN		0b0000000000000001
#define INPUT_ZOOM_OUT		0b0000000000000010
#define INPUT_PAN_UP		0b0000000000000100
#define INPUT_PAN_DOWN		0b0000000000001000
#define INPUT_PAN_LEFT		0b0000000000010000
#define INPUT_PAN_RIGHT		0b0000000000100000
#define INPUT_ROTATE_UP		0b0000000001000000
#define INPUT_ROTATE_DOWN	0b0000000010000000
#define INPUT_ROTATE_LEFT	0b0000000100000000
#define INPUT_ROTATE_RIGHT	0b0000001000000000
#define INPUT_EXIT			0b0000010000000000

typedef uint16_t	keystates;


typedef enum	scop_key
{
	KEY_ZOOM_IN,
	KEY_ZOOM_OUT,
	KEY_PAN_UP,
	KEY_PAN_DOWN,
	KEY_PAN_LEFT,
	KEY_PAN_RIGHT,
	KEY_ROTATE_UP,
	KEY_ROTATE_DOWN,
	KEY_ROTATE_LEFT,
	KEY_ROTATE_RIGHT,
	KEY_EXIT,
	KEY_COUNT,
}				scop_key;

typedef int		scop_keymap[KEY_COUNT];

extern const char*	scop_keystrtab[KEY_COUNT];

typedef struct	scop_settings
{
	scop_keymap	keymap;
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
	keystates		input;
	GLFWwindow		*window;
	map				map;
}				scop;

typedef void 	(*scop_resize_cb)(scop *scop, GLuint width, GLuint height);


int		scop_init(scop *scene);
void	scop_terminate(scop *scene);

void	scop_draw(scop *scene);
int		scop_loop(scop *scene);

int		scop_load_obj_raw(scop *scene, const void *vertices, GLsizeiptr size);
int		scop_load_obj_file(scop *scene, const char *file_path);
