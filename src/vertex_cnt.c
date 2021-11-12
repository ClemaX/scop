#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include <vertex_cnt.h>

#include <logger.h>

static inline int	vertex_cnt_push_raw(vertex_cnt *container,
	const void *vertices, GLsizeiptr count)
{
	int	ret;

	if (container->count + count > container->size)
		ret = vertex_cnt_resize(container, container->count + count);
	else
		ret = 0;

	if (ret == 0)
	{
		memcpy(container->data + container->count, vertices,
			container->vertex_size * count);

		container->count += count;

		debug("vertex count: %zu\n", container->count);
	}

	return ret;
}

int					vertex_cnt_init(vertex_cnt *container, const void *data,
	GLsizeiptr vertex_size, GLsizeiptr size)
{
	int	ret;

	container->data = malloc(vertex_size * size);

	ret = -(container->data == NULL);

	if (ret == 0)
	{
		if (data != NULL)
		{
			memcpy(container->data, data, vertex_size * size);
			container->count = size;
		}
		else
			container->count = 0;
		container->vertex_size = vertex_size;
		container->size = size;
	}
	else
		error("vertex_cnt_init: malloc: %s\n", strerror(errno));

	return ret;
}

void				vertex_cnt_destroy(vertex_cnt *container)
{
	free(container->data);
	bzero(container, sizeof(*container));
}

int					vertex_cnt_cpy(vertex_cnt *dst, const vertex_cnt *src)
{
	int	ret = vertex_cnt_resize(dst, src->size);

	if (ret == 0)
	{
		dst->count = src->count;
		memcpy(src->data, dst->data, src->vertex_size * src->count);
	}

	return ret;
}

int				vertex_cnt_resize(vertex_cnt *container, GLsizeiptr new_size)
{
	void		*data = realloc(container->data, new_size * container->vertex_size);
	const int	ret = -(data == NULL);

	if (ret == 0)
	{
		container->data = data;
		container->size = new_size;
		if (container->count > container->size)
			container->count = container->size;
	}
	return ret;
}

int				vertex_cnt_push(vertex_cnt *container, const void *vec)
{
	return vertex_cnt_push_raw(container, vec, 1);
}

int				vertex_cnt_cat(vertex_cnt *container, const vertex_cnt *other)
{
	return vertex_cnt_push_raw(container, other->data, other->count);
}
