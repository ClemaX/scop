#include <stdlib.h>

#include <string.h>
#include <face_cnt.h>

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
