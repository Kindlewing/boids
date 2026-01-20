#include "base/arena.h"
#include "platform/platform.h"

int main(void) {
	arena *a = arena_create(1024 * 1024); // 1 MB arena
	platform_window *win = platform_create_window(a, 800, 600, "Triangle with EBO");

	// clang-format off
	f32 vertices[] = {
	    // positions      // colors
	    0.0f, 0.5f, 1.0f, 0.0f, 0.0f,   // top, red
	    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // left, green
	    0.5f, -0.5f, 0.0f, 0.0f, 1.0f   // right, blue
	};
	u32 indices[] = {
	    0, 1, 2
	};
	// clang-format on
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex attributes
	// position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// --- Render loop ---
	b8 should_close = false;
	while(!should_close) {
		platform_poll_events(win, &should_close);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		platform_swap_buffers(win);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	platform_close_window(win);
	arena_free(a);

	return 0;
}
