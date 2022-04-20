#pragma once
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Input.h"
#include "Editor.h"
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
	void render();
	std::vector<char> m_pixels;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_texture;
	ShaderProgram m_program;
};