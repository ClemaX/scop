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

	errno = 0;

	do
	{
		len = getline(&line, &size, file);
		if (len > 0)
		{
			if (line[len - 1] == '\n')
				line[--len] = '\0';
			ret = object_exec(object, line);
		}
	} while (len != -1 && ret == 0);

	if (len == -1 && errno != 0)
	{
		error("object_load: getline: %s\n", strerror(errno));
		ret = -1;
	}

	if (ret != 0)
		object_destroy(object);

	free(line);

	return ret;
}

int	object_load_raw(object *object, const void *data, GLsizeiptr size)
{
	int	ret;

	object_init(object);

	ret = vertex_cnt_init(&object->v, data, object->v.vertex_size,
		size / object->v.vertex_size);

	return ret;
}
