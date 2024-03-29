#include <iostream>
#include <fstream>
#include <sdl2/SDL.h>
#include <gl/glew.h>
#include "Utils.h"
#include "Window.h"

namespace Utils
{

    void InitSDL()
    {
        //initializing the SDL library to video only
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
            exit(1);
        }
    }

    void InitGL()
    {

        glewExperimental = true;
        if (glewInit() != GLEW_OK)
        {
            SDL_GL_DeleteContext(Window.GetSDLContext());
            SDL_DestroyWindow(Window.GetSDLWindow());
            std::cout << "GLEW Error: Failed to init" << std::endl;
            SDL_Quit();
            exit(1);
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_TEXTURE_2D);
        glDebugMessageCallback(Utils::MessageCallback, 0);

        //depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //back face removal
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);
    }

    void ParseShader(std::string& filename, std::string& code)
    {
        //stream to read from the file
        std::ifstream inFile(filename, std::ios::in);

        //checking if it was opened
        if (!inFile)
            std::cerr << "ERROR WHILE TRYING TO OPEN " << filename << " FILE\n";

        //reading the code from the file and adding it to the string
        while (inFile.good())
        {
            int c = inFile.get();
            if (!inFile.eof())
                code.push_back((char)c);
        }
        //closing the file
        inFile.close();
    }

    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam)
    {
        const char* _source;
        const char* _type;
        const char* _severity;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UNDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
        }

       // if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
            std::cout << id << ":" << _type << " of " << _severity << " severity, raised from " << _source << ": " << message << "\n";
    }

    uint16_t GetMem(uint16_t buf_big_endian, int start, int count)
    {
        constexpr int opcode_size = 4;
        char* buf = reinterpret_cast<char*>(&buf_big_endian);
        std::swap(buf[0], buf[1]); // little endian conversor

        if (count <= 0 || start + count > opcode_size)
            return 0;
        buf_big_endian <<= (start * opcode_size);
        buf_big_endian >>= (opcode_size * (opcode_size - count));
        return buf_big_endian;
    }

    uint8_t GetBits(uint16_t buf_big_endian, int start, int count)
    {
        constexpr int opcode_size = 4;
        char * buf = reinterpret_cast<char*>(&buf_big_endian);
        std::swap(buf[0], buf[1]); // little endian conversor

        if (count <= 0 || start + count > opcode_size)
            return 0;
        buf_big_endian <<= (start * opcode_size);
        buf_big_endian >>= (opcode_size * (opcode_size - count));
        return (uint8_t)buf_big_endian;
    }
   
}