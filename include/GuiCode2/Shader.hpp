#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils.hpp"

namespace GuiCode
{
    class Shader
    {
    public:
        unsigned int ID;

        Shader(const std::string& vertex, const std::string& frag, const std::string& geo = "");

        inline void Clean()                                    const { glDeleteProgram(ID); };
        inline void Use()                                      const { glUseProgram(ID); };
        inline void SetBool(const char* n, bool v)             const { glUniform1i(glGetUniformLocation(ID, n), (int)v); };
        inline void SetInt(const char* n, int v)               const { glUniform1i(glGetUniformLocation(ID, n), v); };
        inline void SetFloat(const char* n, float v)           const { glUniform1f(glGetUniformLocation(ID, n), v); };
        inline void SetFloatA(const char* n, const float* v, const unsigned int a)  const { glUniform4fv(glGetUniformLocation(ID, n), a, v); };
        inline void SetIntA(const char* n, const int* v, const unsigned int a)  const { glUniform1iv(glGetUniformLocation(ID, n), a, v); };
        inline void SetVec2(const char* n, const glm::vec2& v) const { glUniform2fv(glGetUniformLocation(ID, n), 1, &v[0]); };
        inline void SetVec3(const char* n, const glm::vec3& v) const { glUniform3fv(glGetUniformLocation(ID, n), 1, &v[0]); };
        inline void SetVec4(const char* n, const glm::vec4& v) const { glUniform4fv(glGetUniformLocation(ID, n), 1, &v[0]); };
        inline void SetVec4(const char* n, const Color& v) const { glUniform4fv(glGetUniformLocation(ID, n), 1, &v.r); };
        inline void SetMat2(const char* n, const glm::mat2& m) const { glUniformMatrix2fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
        inline void SetMat3(const char* n, const glm::mat3& m) const { glUniformMatrix3fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
        inline void SetMat4(const char* n, const glm::mat4& m) const { glUniformMatrix4fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
    };
}