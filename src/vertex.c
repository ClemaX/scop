#include <stdlib.h>

#include <vertex.h>

#include <logger.h>

void	vertex_array_init(GLuint *vertex_array_id)
{
	debug("Initializing vertex array object...\n");
	glGenVertexArrays(1, vertex_array_id);
	debug("Binding vertex array object with id %u...\n", *vertex_array_id);
	glBindVertexArray(*vertex_array_id);
	debug("Initialized vertex array object with id %u\n", *vertex_array_id);
}

void	vertex_buffer_init(GLuint *buffer_id)
{
	debug("Initializing vertex buffer...\n");
	glGenBuffers(1, buffer_id);
	debug("Initialized vertex buffer with id %u\n", *buffer_id);
}

void	vertex_buffer_load(GLuint buffer_id, vertex_cnt *container,
	GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	debug("Bound vertex buffer with id %u!\n", buffer_id);

	debug("Buffering vertex data: count: %zu, vertex_size: %zu\n",
		container->count, container->vertex_size);
	glBufferData(
		GL_ARRAY_BUFFER,							// Type
		container->count * container->vertex_size,	// Size
		container->data,							// Data
		usage										// Usage
	);
}

GLsizei	vertex_index_buffer_load(GLuint buffer_id, face_cnt *container,
	GLenum usage)
{
	GLsizei		index_count;
	GLuint		*indices;

	indices = face_cnt_triangulate(container, &index_count);

	if (indices == NULL)
	{
		perror("face_cnt_triangulate");
		index_count = -1;
		goto face_cnt_triangulate_error;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	debug("Bound vertex index buffer with id %u!\n", buffer_id);

	debug("Buffering vertex index data (%d indices)...\n", index_count);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,		// Type
		sizeof(*indices) * index_count,	// Size
		indices,						// Data
		usage							// Usage
	);

	free(indices);

face_cnt_triangulate_error:
	return index_count;
}
