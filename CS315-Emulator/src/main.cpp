#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Input.h"
#include "Editor.h"
#include "Shader.h"

#undef main
int main(void)
	{
	Utils::InitSDL();
	Window.Create(glm::ivec2{ Chip8::WIDTH * 8, Chip8::HEIGHT * 8});
	Utils::InitGL();
	ShaderProgram program;
	GLuint FBO;
	GLuint renderTexture;

	float vertices[] = {
		// positions          // colors           // texture coords
		 1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
	unsigned int texture1;
	// texture 1
	// ---------
	struct color
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 255;
	};

	std::vector<char> pix;

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

			/*
			int pos = y - x;
			if (pos < 0) pos = 64 + pos;

			if (pos % 6 == 0 || pos % 6 == 1 || pos % 6 == 2) back.r = 255;
			if (pos % 6 == 2 || pos % 6 == 3 || pos % 6 == 4) back.g = 255;
			if (pos % 6 == 0 || pos % 6 == 4 || pos % 6 == 5) back.b = 255;*/

			pix.push_back(back.r);
			pix.push_back(back.g);
			pix.push_back(back.b);
			pix.push_back(back.a);
		}
	}

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pix[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	program.Use();
	program.SetIntUniform("ourTexture", 0);

	InputManager.Initialize();
	//Editor.Initialize();
	Chip8 Emulator;

	while (!Window.IsClosed())
	{
		//Editor.StartFrame();
		InputManager.StartFrame();
		InputManager.HandleEvents();
		
		//Window.Update();
		Emulator.Update();
		int x, y;
		SDL_GetWindowSize(Window.GetSDLWindow(), &x, &y);

		glViewport(0, 0, x, y);
		Window.Clear();
		//Editor.Render();
		// Window.SwapBuffers();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		program.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(Window.GetSDLWindow());

	}

	Window.ShutDown();
	//Editor.ShutDown();
	SDL_Quit();
	/*
	std::ifstream f;
	f.open("test_opcode.ch8", std::ios::binary | std::ios::in);

	if (f.bad())
	{
		return 1;
	}

	char buf[2];
	f.seekg(0, f.end);
	int size = f.tellg();
	f.seekg(0, f.beg);
	int seekpos = 0;

	while (seekpos < size)
	{
		f.read(buf, 2);
		std::swap(buf[0], buf[1]); // little endian conversor
		uint16_t opcode = *reinterpret_cast<uint16_t*>(buf);
		
		std::cout << std::hex << seekpos << ": ";
		Disassembler::PrintOpcode(opcode);
		std::cout << std::endl;

		seekpos += 2;
	}
	*/
	return 0;
}