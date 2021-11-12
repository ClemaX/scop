#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

#include <object.h>

#include <vertex_cnt.h>

#include <string.h>
#include <errno.h>

#include <logger.h>

void		object_init(object *object)
{
	matrix_identity4(object->model, 1.0f);

	bzero(&object->v, sizeof(object->v));
	bzero(&object->vn, sizeof(object->vn));
	bzero(&object->vt, sizeof(object->vt));
	bzero(&object->f, sizeof(object->f));

	object->shader_id = 0;

	object->v.vertex_size = sizeof(vec4);
	object->vn.vertex_size = sizeof(vec3);
	object->vt.vertex_size = sizeof(vec3);
}

void		object_destroy(object *object)
{
	if (object->v.size != 0)
		vertex_cnt_destroy(&object->v);
	if (object->vn.size != 0)
		vertex_cnt_destroy(&object->vn);
	if (object->vt.size != 0)
		vertex_cnt_destroy(&object->vt);
	if (object->f.count != 0)
		face_cnt_destroy(&object->f);
}

int			object_copy(object *dest, object *src)
{
	int	ret;

	memcpy(dest->model, src->model, sizeof(src->model));

	ret = vertex_cnt_init(&dest->v, src->v.data,
		src->v.vertex_size, src->v.count);

	if (ret == 0)
		ret = vertex_cnt_init(&dest->vt, src->v.data,
			src->vt.vertex_size, src->vt.count);

	return ret;
}

void		object_set_shader(object *object, GLuint shader_id)
{
	object->shader_id = shader_id;
}

int			object_write(object *object, FILE *file)
{
	for (GLsizeiptr i = 0; i < object->v.count; i++)
	{
		const int	o = i * sizeof(vec4) / sizeof(*object->v.data);

		fprintf(file, "v %f %f %f",
			object->v.data[o + x], object->v.data[o + y],
			object->v.data[o + z]);

		if (object->v.data[o + w] != 1.0f)
			fprintf(file, " %f", object->v.data[o + w]);

		fprintf(file, "\n");
	}

	for (GLsizeiptr i = 0; i < object->vn.count; i++)
	{
		const int	o = i * sizeof(vec3) / sizeof(*object->v.data);

		fprintf(file, "vn %f %f %f\n",
			object->vn.data[o + x], object->vn.data[o + y],
			object->vn.data[o + z]);
	}

	for (GLsizeiptr i = 0; i < object->vt.count; i++)
	{
		const int	o = i * sizeof(vec3) / sizeof(*object->v.data);

		fprintf(file, "vt %f %f",
			object->vt.data[o + x], object->vt.data[o + y]);

		if (object->vt.data[o + z] != 0.0f)
			fprintf(file, " %f", object->vt.data[o + z]);

		fprintf(file, "\n");
	}

	for (GLsizeiptr i = 0; i < object->f.count; i++)
	{
		fprintf(file, "f");

		for (GLsizeiptr j = 0; j < object->f.data[i].count; j++)
		{
			fprintf(file, " %d", object->f.data[i].elems[j].v);
			if (object->f.data[i].elems[j].vn != -1)
			{
				fprintf(file, "/%d", object->f.data[i].elems[j].vn);
				if (object->f.data[i].elems[j].vt != -1)
					fprintf(file, "/%d", object->f.data[i].elems[j].vt);
			}
		}

		fprintf(file, "\n");
	}

	return 0;
}
