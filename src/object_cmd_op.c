#include <stdlib.h>
#include <ctype.h>

#include <object.h>

#include <logger.h>

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
			face->elems[i].vt = 0;
			face->elems[i].vn = 0;

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

int							object_o(object *object, const char *arguments)
{
	int	ret;

	ret = *arguments == '\0';

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 'o' has not been implemented!\n");
	}

	return 0;
}

int							object_g(object *object, const char *arguments)
{
	int ret;

	// TODO: Implement named groups and group their components accordingly
	ret = *arguments == '\0';

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 'g' has not been implemented!\n");
	}

	return 0;
}

int							object_mtllib(object *object, const char *arguments)
{
	int ret;

	// TODO: Implement material file parsing and loading
	ret = *arguments == '\0';

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 'mtllib' has not been implemented!\n");
	}

	return 0;
}

int							object_usemtl(object *object, const char *arguments)
{
	int ret;

	// TODO: Implement material usage
	ret = *arguments == '\0';

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 'usemtl' has not been implemented!\n");
	}

	return 0;
}

int							object_s(object *object, const char *arguments)
{
	int ret;

	// TODO: Implement smoothing groups and shade their components accordingly
	ret = *arguments == '\0';

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 's' has not been implemented!\n");
	}

	return 0;
}

int							object_v(object *object, const char *arguments)
{
	vec4	vertex = {0, 0, 0, 1};
	int		ret = sscanf(arguments, "%f %f %f %f",
		vertex + _v_x, vertex + _v_y, vertex + _v_z, vertex + _v_w);

	ret = ret < 3;
	if (ret == 0)
		vertex_cnt_push(&object->v, &vertex);

	return ret;
}

int							object_vt(object *object, const char *arguments)
{
	vec3	vertex = {0, 0, 0};
	int		ret = sscanf(arguments, "%f %f %f",
		vertex + _v_x, vertex + _v_y, vertex + _v_z);

	ret = ret < 1;
	if (ret == 0)
		vertex_cnt_push(&object->vt, &vertex);

	return ret;
}

int							object_vn(object *object, const char *arguments)
{
	vec3	vertex = {0, 0, 0};
	int		ret = sscanf(arguments, "%f %f %f",
		vertex + _v_x, vertex + _v_y, vertex + _v_z);

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
	{
		for (GLsizei i = 0; i < face.count && ret == 0; ++i) {
			ret = face.elems[i].v < -object->v.count || face.elems[i].v > object->v.count
				|| face.elems[i].vt < -object->vt.count || face.elems[i].vt > object->vt.count
				|| face.elems[i].vn < -object->vn.count || face.elems[i].vn > object->vn.count;
		}

		if (ret == 0) {
			ret = face_cnt_push(&object->f, &face);
		}
	}

	return ret;
}

int							object_l(object *object, const char *arguments)
{
	const GLsizeiptr	point_count = argument_count(arguments);
	int					ret;

	// TODO: Implement smoothing groups and shade their components accordingly
	ret = point_count < 2;

	if (ret == 0)
	{
		(void)object;
		warning("object_cmd_op: operator 'l' has not been implemented!\n");
	}

	return 0;
}
