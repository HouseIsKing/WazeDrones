#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using glm::mat4x4;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

class Shader
{
    unordered_map<string, int> Uniforms;
    static void CompileShader(GLuint shader);
    static void LinkProgram(GLuint program);
public:
    ~Shader();
    Shader(Shader&& other) noexcept = default;
    Shader& operator=(Shader&& other) noexcept = delete;
    Shader(const Shader& other) = default;
    Shader& operator=(const Shader& other) = delete;
    const GLuint Program;
    Shader(const string& vertPath, const string& fragPath);
    void Use() const;
    [[nodiscard]] GLuint GetAttribLocation(const string& name) const;
    [[nodiscard]] GLuint GetUniformBlockIndex(const string& name) const;
    [[nodiscard]] int GetUniformInt(const string& name) const;
    static void SetInt(int posUniform, int value);
    static void SetFloat(int posUniform, float value);
    static void SetVec3(int posUniform, float x, float y, float z);
    static void SetMat4(int posUniform, mat4x4 value);
};