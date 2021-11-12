#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <object.h>

#include <logger.h>

int	object_load(object *object, FILE *file)
{
	char	*line;
	size_t	size;
	ssize_t	len;
	int		ret;

	line = NULL;
	size = 0;
	ret = 0;

	object_init(object);

	do
	{
		len = getline(&line, &size, file);
		if (len > 0)
		{
			line[len - 1] = '\0';
			ret = object_exec(object, line);
		}
	} while (len > 0 && ret == 0);

	if (len == -1 && errno != 0)
	{
		error("object_load: getline: %s\n", strerror(errno));
		ret = -1;
	}

	if (ret != 0)
		object_destroy(object);

	free(line);

	fclose(file);
	return ret;
}

int	object_load_raw(object *object, const void *data, GLsizeiptr size)
{
	const GLsizeiptr	vertex_size = sizeof(vec4);
	int					ret;

	ret = vertex_cnt_init(&object->v, data, vertex_size, size / vertex_size);
	if (ret == 0)
	{
		bzero(&object->vt, sizeof(object->vt));
		bzero(&object->vn, sizeof(object->vn));
		bzero(&object->f, sizeof(object->f));
	}

	return ret;
}
