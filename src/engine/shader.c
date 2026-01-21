#include "engine/shader.h"
#include "base/typedefs.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "third_party/glad/gl.h"

static inline void gl_shader_source_string8(GLuint shader, string8 src) {
	const GLchar *s = (const GLchar *)src.data;
	GLint len = (GLint)src.length;
	glShaderSource(shader, 1, &s, &len);
}

void shader_init(arena *a, shader* s, string8 vertex_path, string8 fragment_path) {
	u32 vertex_fd = open((char*)vertex_path.data, O_RDONLY);
	u32 fragment_fd = open((char*)fragment_path.data, O_RDONLY);
	i64 vertex_size = lseek(vertex_fd, 0, SEEK_END);
	i64 fragment_size = lseek(fragment_fd, 0, SEEK_END);


	lseek(vertex_fd, 0, SEEK_SET);
	lseek(fragment_fd, 0, SEEK_SET);
	
	string8 vertex_src = string8_read_file(a, vertex_fd);
	string8 fragment_src = string8_read_file(a, fragment_fd);

	printf("Vertex size: %llu\n", vertex_size);
	printf("Fragment size: %llu\n", fragment_size);

	s->vertex_id = glCreateShader(GL_VERTEX_SHADER);
	s->fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	
	gl_shader_source_string8(s->vertex_id, vertex_src);
	gl_shader_source_string8(s->fragment_id, fragment_src);

	glCompileShader(s->vertex_id);
	glCompileShader(s->fragment_id);
	s->id = glCreateProgram();
	close(vertex_fd);
	close(fragment_fd);
}
