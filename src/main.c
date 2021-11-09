#include <scop.h>

#include <stdio.h>
#include <stdlib.h>

int			main(void)
{
	scop_settings				settings = (scop_settings){
		.gl_major = 4,
		.gl_minor = 1,
		.anti_aliasing = 4,
		.width = 1280,
		.height = 720,
	};
	GLFWwindow *const			window = scop_init(&settings);
	int							ret;

	ret = -(window == NULL);

	if (ret == 0)
		ret = scop_loop(window);

	return ret;
}
