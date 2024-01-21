#include <scop.h>

#include <string.h>
#include <errno.h>

#include <vertex.h>

#include <logger.h>

static int	scop_buffer_obj(scop_scene *scene)
{
	int ret;

	vertex_buffer_load(scene->vbo_id, &scene->obj.v, GL_STATIC_DRAW);
	scene->vertex_index_count = vertex_index_buffer_load(scene->vibo_id, &scene->obj.f, GL_STATIC_DRAW);

	ret = scene->vertex_index_count < 0;

	if (ret == 0)
		scop_draw(scene);

	return ret;
}

int			scop_load_obj_raw(scop_scene *scene, const void *vertices, GLsizeiptr size)
{
	int	ret;

	ret = object_load_raw(&scene->obj, vertices, size);

	if (ret == 0)
		ret = scop_buffer_obj(scene);

	return ret;
}

int			scop_load_obj_file(scop_scene *scene, const char *file_path)
{
	FILE	*file;
	int		ret;

	file = fopen(file_path, "r");
	ret = -(file == NULL);
	if (ret != 0)
		goto fopen_error;

	ret = object_load(&scene->obj, file);

	fclose(file);

	if (ret != 0)
		goto object_load_error;

	ret = scop_buffer_obj(scene);

	goto done;

fopen_error:
	error("fopen: %s: %s\n", file_path, strerror(errno));

object_load_error:
	error("object_load: Could not load object from filepath '%s'\n", file_path);

done:
	return ret;
}
