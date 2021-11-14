#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include <shader.h>
#include <file.h>
#include <logger.h>

static void		shader_perror(const char *name, GLuint shader_id, GLint log_length)
{
	char	*log = malloc(sizeof(*log) * log_length + 1);

	if (log != NULL)
	{
		glGetShaderInfoLog(shader_id, log_length, NULL, log);
		error("shader_load: %s: %s\n", name, log);
	}
	else
		error("shader_load: malloc: %s\n", strerror(errno));
}

static void		shader_program_perror(const char *vertex_name,
	const char *fragment_name, GLuint program_id, GLint log_length)
{
	char	*log = malloc(sizeof(*log) * log_length + 1);

	if (log != NULL)
	{
		glGetShaderInfoLog(program_id, log_length, NULL, log);
		error("shader_link: %s, %s: %s\n", vertex_name, fragment_name, log);
	}
	else
		error("shader_link: malloc: %s\n", strerror(errno));
}

static GLuint	shader_load_mem(const char *name, const GLchar *data,
	GLint size, GLenum shader_type)
{
	GLuint	id;
	GLint	status;
	GLint	log_length;

	id = glCreateShader(shader_type);

	if (id != 0)
	{
		glShaderSource(id, 1, &data, &size);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length == 0)
			debug("Compiled shader successfully!\n");
		else
		{
			shader_perror(name, id, log_length);
			glDeleteShader(id);
			id = 0;
		}
	}
	else
		error("shader_load: Could not create shader of type %d\n", shader_type);

	return id;
}

static GLuint	shader_load_file(const char *file, GLenum shader_type)
{
	int		fd;
	GLuint	id;

	fd = open(file, O_RDONLY);

	id = fd != -1;
	if (id != 0)
	{
		// TODO: Check file size before loading to not exceed GL_INT_MAX
		void		*data;
		const off_t	size = file_map(fd, &data);

		id = size != -1;
		if (id != 0)
		{
			debug("Loading shader from '%s' [%jd bytes]...\n", file, (long)size);

			id = shader_load_mem(file, data, size, shader_type);

			file_unmap(&data, size);
		}
		else
			error("file_map: %s\n", strerror(errno));

		close(fd);
	}
	else
		error("shader_load: %s: %s\n", file, strerror(errno));

	return id;
}

static GLuint	shader_link(const char *vertex_name, const char *fragment_name,
	GLuint vertex_shader_id, GLuint fragment_shader_id)
{
	GLuint	program_id;
	GLint	status;
	GLint	log_length;

	program_id = glCreateProgram();

	if (program_id != 0)
	{
		debug("Linking shader program...\n");

		glAttachShader(program_id, vertex_shader_id);
		glAttachShader(program_id, fragment_shader_id);

		glLinkProgram(program_id);

		glGetProgramiv(program_id, GL_LINK_STATUS, &status);
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

		glDetachShader(program_id, vertex_shader_id);
		glDetachShader(program_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		if (log_length != 0)
		{
			shader_program_perror(vertex_name, fragment_name, program_id,
				log_length);

			glDeleteProgram(program_id);
			program_id = 0;
		}
		else
			debug("Linked program succesfully!\n");
	}
	else
		error("shader_link: Could not create shader program!\n");

	return program_id;
}

GLuint			shader_load(const char *vertex_file, const char *fragment_file)
{
	GLuint	vertex_shader_id;
	GLuint	fragment_shader_id;
	GLuint	program_id;

	vertex_shader_id = shader_load_file(vertex_file, GL_VERTEX_SHADER);

	if (vertex_shader_id != 0)
	{
		fragment_shader_id = shader_load_file(fragment_file, GL_FRAGMENT_SHADER);

		if (fragment_shader_id != 0)
		{
			program_id = shader_link(vertex_file, fragment_file,
				vertex_shader_id, fragment_shader_id);

			if (program_id != 0)
				return program_id;

			glDeleteShader(fragment_shader_id);
		}

		glDeleteShader(vertex_shader_id);
	}
	return 0;
}
