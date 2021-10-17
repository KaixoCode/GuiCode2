#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils/Color.hpp"

namespace GuiCode
{
    class Shader
    {
    public:
        unsigned int ID;

        Shader(const std::string& vertex, const std::string& frag, const std::string& geo = "");

        inline void Clean() { glDeleteProgram(ID); };
        inline void Use() { glUseProgram(ID); };
        inline void SetBool(GLint n, bool v) { glUniform1i(n, (int)v); };
        inline void SetInt(GLint n, int v) { glUniform1i(n, v); };
        inline void SetFloat(GLint n, float v) { glUniform1f(n, v); };
        inline void SetFloatA(GLint n, const float* v, const unsigned int a) { glUniform4fv(n, a, v); };
        inline void SetIntA(GLint n, const int* v, const unsigned int a) { glUniform1iv(n, a, v); };
        inline void SetVec2(GLint n, const glm::vec2& v) { glUniform2fv(n, 1, &v.x); };
        inline void SetVec3(GLint n, const glm::vec3& v) { glUniform3fv(n, 1, &v.x); };
        inline void SetVec4(GLint n, const glm::vec4& v) { glUniform4fv(n, 1, &v.x); };
        inline void SetVec4(GLint n, const Color& v) { glUniform4fv(n, 1, &v.r); };
        inline void SetMat2(GLint n, const glm::mat2& m) { glUniformMatrix2fv(n, 1, GL_FALSE, &m[0][0]); };
        inline void SetMat3(GLint n, const glm::mat3& m) { glUniformMatrix3fv(n, 1, GL_FALSE, &m[0][0]); };
        inline void SetMat4(GLint n, const glm::mat4& m) { glUniformMatrix4fv(n, 1, GL_FALSE, &m[0][0]); };
    };
}