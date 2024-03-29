#include "Renderer.h"
#include <glm/gtc/random.hpp>

void Renderer::initialize()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture 
// -------------------------

	// this loop will calculate the color of each of the squares (6x6 image)
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			color back;
			if ((x + y) % 2 == 0)
			{
				back.r = back.g = back.b = 255;
			}

			m_pixels.push_back(back.r);
			m_pixels.push_back(back.g);
			m_pixels.push_back(back.b);
			m_pixels.push_back(back.a);
		}
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_program.Use();
	m_program.SetIntUniform("ourTexture", 0);
}

void Renderer::update(std::array<std::bitset<Chip8::WIDTH>, Chip8::HEIGHT> & FrameBuffer)
{
	int px = 0;
	for (int i = 0; i < Chip8::HEIGHT; i++)
	{
		for (int j = 0; j < Chip8::WIDTH; j++)
		{
			int set = FrameBuffer[i][j];

			m_pixels[px++] = (set * m_white.r) + (!set * m_black.r);
			m_pixels[px++] = (set * m_white.g) + (!set * m_black.g);
			m_pixels[px++] = (set * m_white.b) + (!set * m_black.b);
			m_pixels[px++] = (set * m_white.a) + (!set * m_black.a);
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);
}

void Renderer::render()
{
	glm::vec4 clear = m_black / 255.0f;
	glClearColor(clear.r, clear.g, clear.b, clear.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int x, y;
	SDL_GetWindowSize(Window.GetSDLWindow(), &x, &y);
	glViewport(0, 0, x, y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	m_program.Use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
