#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils.hpp"

namespace GuiCode
{
    class Shader
    {
        std::map<const char*, GLint> m_Uniforms;
    public:
        unsigned int ID;

        Shader(std::vector<const char*>&&, const std::string& vertex, const std::string& frag, const std::string& geo = "");

        inline void Clean() { glDeleteProgram(ID); };
        inline void Use() { glUseProgram(ID); };
        inline void SetBool(const char* n, bool v) { glUniform1i(m_Uniforms[n], (int)v); };
        inline void SetInt(const char* n, int v) { glUniform1i(m_Uniforms[n], v); };
        inline void SetFloat(const char* n, float v) { glUniform1f(m_Uniforms[n], v); };
        inline void SetFloatA(const char* n, const float* v, const unsigned int a) { glUniform4fv(m_Uniforms[n], a, v); };
        inline void SetIntA(const char* n, const int* v, const unsigned int a) { glUniform1iv(m_Uniforms[n], a, v); };
        inline void SetVec2(const char* n, const glm::vec2& v) { glUniform2fv(m_Uniforms[n], 1, &v.x); };
        inline void SetVec3(const char* n, const glm::vec3& v) { glUniform3fv(m_Uniforms[n], 1, &v.x); };
        inline void SetVec4(const char* n, const glm::vec4& v) { glUniform4fv(m_Uniforms[n], 1, &v.x); };
        inline void SetVec4(const char* n, const Color& v) { glUniform4fv(m_Uniforms[n], 1, &v.r); };
        inline void SetMat2(const char* n, const glm::mat2& m) { glUniformMatrix2fv(m_Uniforms[n], 1, GL_FALSE, &m[0][0]); };
        inline void SetMat3(const char* n, const glm::mat3& m) { glUniformMatrix3fv(m_Uniforms[n], 1, GL_FALSE, &m[0][0]); };
        inline void SetMat4(const char* n, const glm::mat4& m) { glUniformMatrix4fv(m_Uniforms[n], 1, GL_FALSE, &m[0][0]); };
    };
}