#include <vertex.h>

#include <logger.h>

void	vertex_array_object(GLuint *vertex_array_id)
{
	debug("Initializing vertex array object...\n");
	glGenVertexArrays(1, vertex_array_id);
	debug("Binding vertex array object with id %u...\n", *vertex_array_id);
	glBindVertexArray(*vertex_array_id);
	debug("Initialized vertex array object with id %u\n", *vertex_array_id);
}

void	vertex_buffer(GLuint *vertex_buffer_id)
{
	debug("Initializing vertex buffer...\n");
	glGenBuffers(1, vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer_id);
	debug("Initialized vertex buffer with id %u\n", *vertex_buffer_id);
}

void	vertex_buffer_data(vertex_cnt *container, GLenum usage)
{
	debug("Buffering vertex data: count: %zu, vertex_size: %zu\n",
		container->count, container->vertex_size);
	glBufferData(GL_ARRAY_BUFFER, container->count * container->vertex_size,
		container->data, usage);
}
