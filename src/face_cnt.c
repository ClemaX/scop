#include <stdlib.h>
#include <string.h>

#include <vector.h>
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

GLuint	*face_cnt_triangulate(face_cnt *container, GLsizeiptr vertex_count, GLsizei *index_count)
{
	GLuint				*indices;
	size_t				count;

	count = 0;

	for (GLsizei i = 0; i < container->count; ++i)
	{
		if (container->data[i].count == 3)
			count += 3;
		else if (container->data[i].count == 4)
			count += 6;
	}

	indices = malloc(sizeof(*indices) * count);

	if (indices == NULL)
		goto malloc_error;

	count = 0;

	// TODO: Check for overflow of GLint indices
	// TODO: Ensure in object parser that vertex index is not zero nor out of bounds
	for (GLsizei face_index = 0; face_index < container->count; ++face_index)
	{
		GLuint	face_indices[4];

		for (GLsizei elem_index = 0; elem_index < container->data[face_index].count; ++elem_index) {
			if (container->data[face_index].elems[elem_index].v > 0)
				face_indices[elem_index] = container->data[face_index].elems[elem_index].v - 1;
			else
				face_indices[elem_index] = vertex_count + container->data[face_index].elems[elem_index].v;
		}

		if (container->data[face_index].count >= 3)
		{
			// Add indices for first triangle vertices (0 1 2)
			indices[count++] = face_indices[0];
			indices[count++] = face_indices[1];
			indices[count++] = face_indices[2];
		}

		if (container->data[face_index].count == 4)
		{
			// Add indices for second triangle vertices (2 3 0)
			indices[count++] = face_indices[2];
			indices[count++] = face_indices[3];
			indices[count++] = face_indices[0];
		}
	}

	*index_count = (GLsizei)count;

malloc_error:
	return indices;
}
