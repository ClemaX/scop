#include <stdio.h>
#include <stdlib.h>

#include <scop.h>

#include <logger.h>

static inline int	invalid_arguments(const char *name)
{
	error("Usage: %s file.obj\n", name);
	return 1;
}

int					main(int ac, const char **av)
{
	scop	scene = (scop){
		.settings = {
			.gl_major = 4,
			.gl_minor = 1,
			.anti_aliasing = 4,
			.width = 1280,
			.height = 720,
		}
	};
	int		ret;

	if (ac < 2)
		return invalid_arguments(av[0]);

	ret = scop_init(&scene);

	scop_load_obj_file(&scene, av[1]);

#ifdef DEBUG
	object_write(&scene.obj, stderr);
#endif

	if (ret == 0)
	{
		ret = scop_loop(&scene);
		scop_terminate(&scene);
	}

	return ret;
}
