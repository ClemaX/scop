#include <scop.h>

#include <stdio.h>
#include <stdlib.h>

int			main(void)
{
	scop_settings		settings = (scop_settings){
		.gl_major = 4,
		.gl_minor = 1,
		.anti_aliasing = 4,
		.width = 1280,
		.height = 720,
	};
	GLuint				shader_program_id;
	GLFWwindow *const	window = scop_init(&settings, &shader_program_id);
	int					ret;

	ret = -(window == NULL);

	if (ret == 0)
		ret = scop_loop(window, shader_program_id);

	return ret;
}
