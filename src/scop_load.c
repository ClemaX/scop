#include <scop.h>

#include <string.h>
#include <errno.h>

#include <vertex.h>

#include <logger.h>

static void	scop_buffer_obj(scop *scene)
{
	vertex_buffer_data(scene->vb_id, &scene->obj.v, GL_STATIC_DRAW);
	scop_draw(scene);
}

int			scop_load_obj_raw(scop *scene, const void *vertices, GLsizeiptr size)
{
	int	ret;

	ret = object_load_raw(&scene->obj, vertices, size);

	if (ret == 0)
		scop_buffer_obj(scene);

	return ret;
}

int			scop_load_obj_file(scop *scene, const char *file_path)
{
	FILE	*file;
	int		ret;

	file = fopen(file_path, "r");
	ret = -(file == NULL);
	if (ret == 0)
	{
		ret = object_load(&scene->obj, file);

		fclose(file);

		scop_buffer_obj(scene);
	}
	else
		error("fopen: %s: %s\n", file_path, strerror(errno));

	return ret;
}
