#include <scop.h>

#include <stdio.h>
#include <stdlib.h>

#include <logger.h>

static void	show_usage(const char *name)
{
	fprintf(stdout, "Usage: %s OBJECT_FILE\n", name);
}

static int	invalid_arguments(const char *name)
{
	fputs("scop: Missing object filepath!\n", stderr);
	show_usage(name);
	return 2;
}

int			main(int ac, const char **av)
{
	scop_scene	scene = (scop_scene){
		.settings = {
			.keymap = {
				[KEY_ZOOM_IN] = GLFW_KEY_W,
				[KEY_ZOOM_OUT] = GLFW_KEY_S,

				[KEY_PAN_UP] = GLFW_KEY_UP,
				[KEY_PAN_DOWN] = GLFW_KEY_DOWN,
				[KEY_PAN_LEFT] = GLFW_KEY_LEFT,
				[KEY_PAN_RIGHT] = GLFW_KEY_RIGHT,

				[KEY_ROTATE_UP] = GLFW_KEY_SPACE,
				[KEY_ROTATE_DOWN] = GLFW_KEY_LEFT_SHIFT,
				[KEY_ROTATE_LEFT] = GLFW_KEY_A,
				[KEY_ROTATE_RIGHT] = GLFW_KEY_D,

				[KEY_EXIT] = GLFW_KEY_ESCAPE,
			},
			.gl_major = 4,
			.gl_minor = 1,
			.anti_aliasing = 4,
			.width = 1280,
			.height = 720,
			.vertex_shader = SHADER_DIR"/vertex.glsl",
			.fragment_shader = SHADER_DIR"/fragment.glsl",
		}
	};
	int		ret;

	if (ac < 2)
	{
		ret = invalid_arguments(av[0]);
		goto scop_init_error;
	}

	ret = scop_init(&scene);
	if (ret != 0)
		goto scop_init_error;

	ret = scop_load_obj_file(&scene, av[1]);
	if (ret != 0)
		goto scop_load_obj_error;

	//debug_obj("scene.obj", scene.obj);

	ret = scop_loop(&scene);

scop_load_obj_error:
	scop_terminate(&scene);


scop_init_error:
	return ret;
}
