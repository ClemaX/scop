#include <scop.h>

#include <stdio.h>
#include <stdlib.h>

int			main(void)
{
	scop	scene = (scop){
		.settings = {
			.gl_major = 4,
			.gl_minor = 1,
			.anti_aliasing = 4,
			.width = 1280,
			.height = 720,
			.vertex_shader = SHADER_DIR"/vertex.glsl",
			.fragment_shader = SHADER_DIR"/fragment.glsl",
		}
	};
	int		ret = scop_init(&scene);

	if (ret == 0)
	{
		ret = scop_load_obj_file(&scene, "./resources/triangle.obj");

#ifdef DEBUG
		object_write(&scene.obj, stderr);
#endif
		ret = scop_loop(&scene);
		scop_terminate(&scene);
	}

	return ret;
}
