#pragma once
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Input.h"
#include "Shader.h"

struct color
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;
};

struct Renderer
{
	void initialize();
	void update(std::array<std::bitset<Chip8::WIDTH>, Chip8::HEIGHT> & mFrameBuffer);
	void render();
	std::vector<char> m_pixels;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_texture;
	ShaderProgram m_program;

	glm::vec4 m_black = { 10,10,70, 255 };
	glm::vec4 m_white = { 200, 200, 255, 255 };
};