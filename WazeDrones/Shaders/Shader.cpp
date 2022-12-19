#include "Shader.h"

#include <array>
#include <fstream>
#include <sstream>

/**
 * \brief Compiles an OpenGL shader
 * \param shader Shader ID
 */
void Shader::CompileShader(const GLuint shader)
{
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        std::array<GLchar, 512> infoLog{0};
        glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog.data() << endl;
    }
}

/**
 * \brief Links an OpenGL program
 * \param program Program ID
 */
void Shader::LinkProgram(const GLuint program)
{
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        std::array<GLchar, 512> infoLog{0};
        glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << endl;
    }
}

/**
 * \brief Constructs a shader from a file
 * \param vertPath Vertex shader path.
 * \param fragPath Fragment shader path.
 */
Shader::Shader(const string& vertPath, const string& fragPath) : Program(glCreateProgram())
{
    string vertCode;
    string fragCode;
    ifstream vertFile;
    ifstream fragFile;

    vertFile.exceptions(ifstream::badbit);
    fragFile.exceptions(ifstream::badbit);

    try
    {
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        stringstream vertStream;
        stringstream fragStream;

        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertCode = vertStream.str();
        fragCode = fragStream.str();
    }
    catch (ifstream::failure& /*e*/)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const char* vertShaderCode = vertCode.c_str();
    const char* fragShaderCode = fragCode.c_str();

    GLuint vertShader;
    GLuint fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderCode, nullptr);
    CompileShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderCode, nullptr);
    CompileShader(fragShader);
    glAttachShader(Program, vertShader);
    glAttachShader(Program, fragShader);
    LinkProgram(Program);
    glDetachShader(Program, vertShader);
    glDetachShader(Program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    GLint uniformCount;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &uniformCount);
    for (GLint i = 0; i < uniformCount; i++)
    {
        GLsizei nameLength;
        GLsizei size;
        GLenum type;
        std::array<GLchar, 100> name{0};
        glGetActiveUniform(Program, static_cast<GLuint>(i), sizeof name, &nameLength, &size, &type, name.data());
        Uniforms.emplace(name.data(),glGetUniformLocation(Program, name.data()));
    }
}

/**
 * \brief Uses the shader
 */
void Shader::Use() const
{
    glUseProgram(Program);
}

/**
 * \brief Gets attribute ID
 * \param name Name of attribute
 * \return ID of attribute
 */
GLuint Shader::GetAttribLocation(const string& name) const
{
    return glGetAttribLocation(Program, name.c_str());
}

GLuint Shader::GetUniformBlockIndex(const string& name) const
{
    return glGetUniformBlockIndex(Program, name.c_str());
}

int Shader::GetUniformInt(const string& name) const
{
    return Uniforms.at(name);
}

void Shader::SetInt(const int posUniform, const int value)
{
    glUniform1i(posUniform, value);
}

void Shader::SetFloat(const int posUniform, const float value)
{
    glUniform1f(posUniform, value);
}

void Shader::SetVec3(const int posUniform, const float x, const float y, const float z)
{
    glUniform3f(posUniform, x, y, z);
}

void Shader::SetMat4(const int posUniform, mat4x4 value)
{
    glUniformMatrix4fv(posUniform, 1, GL_FALSE, &value[0][0]);
}

Shader::~Shader()
{
    glDeleteProgram(Program);
}

