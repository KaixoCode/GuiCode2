#pragma once
#include "GuiCode/pch.hpp"

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    void Clean()                                    const { glDeleteProgram(ID); };
    void Use()                                      const { glUseProgram(ID); };
    void SetBool(const char* n, bool v)             const { glUniform1i(glGetUniformLocation(ID, n), (int)v); };
    void SetInt(const char* n, int v)               const { glUniform1i(glGetUniformLocation(ID, n), v);  };
    void SetFloat(const char* n, float v)           const { glUniform1f(glGetUniformLocation(ID, n), v); };
    void SetVec2(const char* n, const glm::vec2& v) const { glUniform2fv(glGetUniformLocation(ID, n), 1, &v[0]); };
    void SetVec3(const char* n, const glm::vec3& v) const { glUniform3fv(glGetUniformLocation(ID, n), 1, &v[0]); };
    void SetVec4(const char* n, const glm::vec4& v) const { glUniform4fv(glGetUniformLocation(ID, n), 1, &v[0]); };
    void SetVec4(const char* n, const Color& v) const { glUniform4fv(glGetUniformLocation(ID, n), 1, &v.x); };
    void SetMat2(const char* n, const glm::mat2& m) const { glUniformMatrix2fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
    void SetMat3(const char* n, const glm::mat3& m) const { glUniformMatrix3fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
    void SetMat4(const char* n, const glm::mat4& m) const { glUniformMatrix4fv(glGetUniformLocation(ID, n), 1, GL_FALSE, &m[0][0]); };
};