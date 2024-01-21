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
/* 
void	vertex_index_buffer_init(GLuint *buffer_id)
{
	debug("Initializing vertex index buffer...\n");
	glGenBuffers(1, buffer_id);
	debug("Initialized vertex index buffer with id %u\n", *buffer_id);
} */

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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	debug("Bound vertex index buffer with id %u!\n", buffer_id);

	index_count = 0;

	for (GLsizei i = 0; i < container->count; ++i) {
		if (container->data[i].count == 3)
			index_count += container->data[i].count;
		else if (container->data[i].count == 4)
			index_count += (container->data[i].count - 2) * 3;
		// TODO: Either support or prevent faces with more than 4 vertex indices
	}

	debug("Allocating temporary index buffer for %d indices...\n", index_count);

	indices = malloc(sizeof(*indices) * index_count);

	index_count = 0;

	for (GLsizei i = 0; i < container->count; ++i) {
		debug("face #%d:\n", i);
		if (container->data[i].count == 3)
		{
			// Add indices for simple triangle vertices
			for (GLsizeiptr j = 0; j < container->data[i].count; ++j) {
				debug("index #%d: %d\n", index_count, container->data[i].elems[j].v - 1);
				indices[index_count++] = container->data[i].elems[j].v - 1;
			}
		}
		else if (container->data[i].count == 4)
		{
			// Add indices for first triangle vertices (0 1 2)
			indices[index_count++] = container->data[i].elems[0].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);
			indices[index_count++] = container->data[i].elems[1].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);
			indices[index_count++] = container->data[i].elems[2].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);

			// Add indices for second triangle vertices (2 3 0)
			indices[index_count++] = container->data[i].elems[2].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);
			indices[index_count++] = container->data[i].elems[3].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);
			indices[index_count++] = container->data[i].elems[0].v - 1;
			debug("index #%d: %d\n", index_count - 1, indices[index_count - 1]);
		}
	}
	
	debug("Buffering vertex index data...\n");

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,		// TYPE
		sizeof(*indices) * index_count,	// Size
		indices,						// Data
		usage							// Usage
	);

	free(indices);

	return index_count;
}
