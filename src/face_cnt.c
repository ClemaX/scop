#include <stdlib.h>

#include <string.h>
#include <face_cnt.h>

#include <logger.h>

int		face_cnt_init(face_cnt *container, GLintptr size)
{
	int	ret;

	container->data = malloc(sizeof(*container->data) * size);

	ret = -(container->data == NULL);
	if (ret == 0)
	{
		container->count = 0;
		container->size = size;
	}

	return ret;
}

void	face_cnt_destroy(face_cnt *container)
{
	for (GLsizeiptr i = 0; i < container->count; i++)
		face_destroy(container->data + i);

	free(container->data);

	bzero(container, sizeof(*container));
}

int		face_cnt_resize(face_cnt *container, GLsizeiptr new_size)
{
	face	*data;
	int		ret;

	data = realloc(container->data, sizeof(*data) * new_size);

	ret = -(data == NULL);
	if (ret == 0)
	{
		container->data = data;
		container->size = new_size;
	}

	return ret;
}

int		face_cnt_push(face_cnt *container, const face *face)
{
	int	ret;

	if (container->count == container->size)
		ret = face_cnt_resize(container, container->size + 1);
	else
		ret = 0;

	if (ret == 0)
		container->data[container->count++] = *face;

	return ret;
}

GLuint	*face_cnt_triangulate(face_cnt *container, GLsizei *index_count)
{
	GLuint				*indices;
	size_t				count;

	count = 0;

	for (GLsizei i = 0; i < container->count; ++i) {
		if (container->data[i].count == 3)
			count += container->data[i].count;
		else if (container->data[i].count == 4)
			count += 6;
	}

	indices = malloc(sizeof(*indices) * count);

	if (indices == NULL)
		goto malloc_error;

	count = 0;

	// TODO: Check for overflow of GLint indices
	// TODO: Map negative values relative to the end of the vertices container
	for (GLsizei i = 0; i < container->count; ++i) {
		debug("face #%d:\n", i);
		if (container->data[i].count >= 3)
		{
			// Add indices for first triangle vertices (0 1 2)
			indices[count++] = container->data[i].elems[0].v - 1;
			indices[count++] = container->data[i].elems[1].v - 1;
			indices[count++] = container->data[i].elems[2].v - 1;
		}
		if (container->data[i].count == 4)
		{
			// Add indices for second triangle vertices (2 3 0)
			indices[count++] = container->data[i].elems[2].v - 1;
			indices[count++] = container->data[i].elems[3].v - 1;
			indices[count++] = container->data[i].elems[0].v - 1;
		}
	}

	*index_count = (GLsizei)count;

malloc_error:
	return indices;
}
