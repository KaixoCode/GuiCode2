#include "GuiCode2/Shader.hpp"

namespace GuiCode
{
    static void CheckCompileErrors(unsigned int shader, std::string type)
    {
        int _success;
        char _infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &_success);
            if (!_success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, _infoLog);
                LOG("ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << _infoLog
                    << "\n -- --------------------------------------------------- -- "
                    << std::endl);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &_success);
            if (!_success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, _infoLog);
                LOG("ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << _infoLog
                    << "\n -- --------------------------------------------------- -- "
                    << std::endl);
            }
        }
    }

    static std::string LoadFile(const char* path)
    {
        if (path == nullptr)
            return "";

        try {
            std::ifstream _file;
            std::stringstream _stream;
            _file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            _file.open(path);
            _stream << _file.rdbuf();
            _file.close();
            return _stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            LOG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        }
    }

    Shader::Shader(const std::string& vertex, const std::string& frag, const std::string& geo)
    {
        ID = glCreateProgram();

        const char* _vShaderCode = vertex.c_str();
        unsigned int _vertex = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(_vertex, 1, &_vShaderCode, NULL);
        glCompileShader(_vertex);
        CheckCompileErrors(_vertex, "VERTEX");
        glAttachShader(ID, _vertex);

        const char* _fShaderCode = frag.c_str();
        unsigned int _fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(_fragment, 1, &_fShaderCode, NULL);
        glCompileShader(_fragment);
        CheckCompileErrors(_fragment, "FRAGMENT");
        glAttachShader(ID, _fragment);

        unsigned int _geometry;
        if (!geo.empty())
        {
            const char* _gShaderCode = geo.c_str();
            _geometry = glCreateShader(GL_GEOMETRY_SHADER);

            glShaderSource(_geometry, 1, &_gShaderCode, NULL);
            glCompileShader(_geometry);
            CheckCompileErrors(_geometry, "GEOMETRY");
            glAttachShader(ID, _geometry);
        }

        glLinkProgram(ID);
        CheckCompileErrors(ID, "PROGRAM");

        glDeleteShader(_vertex);
        glDeleteShader(_fragment);
        if (!geo.empty())
            glDeleteShader(_geometry);
    }
}