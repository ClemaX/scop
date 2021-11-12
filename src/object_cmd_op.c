#include <stdlib.h>
#include <ctype.h>

#include <object.h>

static inline GLsizeiptr	argument_count(const char *arguments)
{
	GLsizeiptr	count;

	count = 0;
	while (*arguments)
	{
		while (*arguments != '\0' && !isspace(*arguments))
			arguments++;

		while (isspace(*arguments))
				arguments++;

		count++;
	};

	return count;
}

static inline int			face_parse(face *face, const char *arguments)
{
	int			ret;
	GLsizeiptr	i;

	face->count = argument_count(arguments);

	ret = face->count < 3;

	if (ret == 0)
	{
		face->elems = malloc(sizeof(*face->elems) * face->count);

		ret = -(face->elems == NULL);

		for (i = 0; ret == 0 && i < face->count; i++)
		{
			// FIXME: Use another value for no-value
			// FIXME: Apply bound checking
			// FIXME: Negative indices are relative to the end
			face->elems[i].vt = -1;
			face->elems[i].vn = -1;
			ret = sscanf(arguments, "%u/%u/%u", &face->elems[i].v,
				&face->elems[i].vt, &face->elems[i].vn) < 1;

			if (ret == 0)
			{
				while (*arguments && !isspace(*arguments))
					arguments++;
				while (isspace(*arguments))
					arguments++;
			}
		}
		if (ret != 0)
		{
			free(face->elems);
			face->elems = NULL;
			face->count = 0;
		}
	}
	return ret;
}

int							object_v(object *object, const char *arguments)
{
	vec4	vertex = {0, 0, 0, 1};
	int		ret = sscanf(arguments, "%f %f %f %f",
		vertex + x, vertex + y, vertex + z, vertex + w);

	ret = ret < 3;
	if (ret == 0)
		vertex_cnt_push(&object->v, &vertex);

	return ret;
}

int							object_vt(object *object, const char *arguments)
{
	vec3	vertex = {0, 0, 0};
	int		ret = sscanf(arguments, "%f %f %f",
		vertex + x, vertex + y, vertex + z);

	ret = ret < 1;
	if (ret == 0)
		vertex_cnt_push(&object->vt, &vertex);

	return ret;
}

int							object_vn(object *object, const char *arguments)
{
	vec3	vertex = {0, 0, 0};
	int		ret = sscanf(arguments, "%f %f %f",
		vertex + x, vertex + y, vertex + z);

	ret = ret < 1;
	if (ret == 0)
		vertex_cnt_push(&object->vn, &vertex);

	return ret;
}

int							object_f(object *object, const char *arguments)
{
	face	face;
	int		ret;

	ret = face_parse(&face, arguments);

	if (ret == 0)
		ret = face_cnt_push(&object->f, &face);

	return ret;
}
