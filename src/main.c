#include "base/arena.h"
#include "base/string8.h"
#include "platform/platform.h"
#include "render/shader.h"
#include <assert.h>
#include <GL/gl.h>

#define WINDOW_W 1200
#define WINDOW_H 1200

int main(void) {
	arena *a = arena_create(1024 * 1024);
	assert(a && "Failed to create arena");

	spark_window *win = spark_create_window(
	 a, WINDOW_W, WINDOW_H, string8_lit("Triangle with EBO"));
	assert(win && "Failed to create platform window");

	shader s = { 0 };
	string8 vertex_file_path = string8_lit("assets/shaders/vertex.glsl");
	string8 fragment_file_path = string8_lit("assets/shaders/fragment.glsl");

	shader_init(a, &s, vertex_file_path, fragment_file_path);
	assert(s.id != 0 && "Shader program ID is 0");
	assert(s.vertex_id != 0 && "Vertex shader ID is 0");
	assert(s.fragment_id != 0 && "Fragment shader ID is 0");

	glAttachShader(s.id, s.vertex_id);
	glAttachShader(s.id, s.fragment_id);
	glLinkProgram(s.id);

	GLint success;
	glGetProgramiv(s.id, GL_LINK_STATUS, &success);
	assert(success && "Shader program failed to link");
	glUseProgram(s.id);
	// clang-format off
    f32 vertices[] = {
         0.5f,  0.5f, 0.0f,  // 0 top right
         0.5f, -0.5f, 0.0f,  // 1 bottom right
        -0.5f, -0.5f, 0.0f,  // 2 bottom left
        -0.5f,  0.5f, 0.0f   // 3 top left
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3 
    };
	// clang-format on

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	assert(VAO != 0 && "Failed to generate VAO");
	assert(VBO != 0 && "Failed to generate VBO");
	assert(EBO != 0 && "Failed to generate EBO");

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
	 GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(
	 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	b8 should_close = false;
	while(!should_close) {
		spark_poll_events(win, &should_close);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		spark_swap_buffers(win);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	spark_close_window(win);
	arena_free(a);
	return 0;
}
