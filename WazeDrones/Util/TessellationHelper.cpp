//
// Created by amit on 4/21/2022.
//

#include "TessellationHelper.h"
#include <ranges>

TessellationHelper::~TessellationHelper()
{
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
}

bool TessellationHelper::HasInitialized() const
{
	return HasInit;
}

TessellationHelper::TessellationHelper(Shader* shader) : Vbo(0), Vao(0), Ebo(0), TheShader(shader), HasInit(false), TrianglesCount(0)
{
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);
    TheShader->Use();
    PositionUniform = TheShader->GetUniformInt("transformationMatrix");
    TessellationTransforms.emplace_back();
}

Transform& TessellationHelper::GetTransform(const size_t id)
{
	return TessellationTransforms.at(id);
}

uint32_t TessellationHelper::AddVertex(const Vertex& vertex)
{
	Vertices.push_back(vertex);
	return static_cast<uint32_t>(Vertices.size() - 1);
}

void TessellationHelper::AddTriangle(const uint32_t triangle)
{
	TriangleIndices.push_back(triangle);
}

mat4x4 TessellationHelper::GetTransformationMatrix(const size_t id)
{
	const mat4x4 helper = TessellationTransforms.at(0).GetTransformMatrix();
	if (id == 0)
	{
		return helper;
	}
	return helper * TessellationTransforms.at(id).GetTransformMatrix();
}

size_t TessellationHelper::GetCurrentTriangleCount() const
{
	return TriangleIndices.size();
}

/*
void TessellationHelper::changeVertex(uint16_t vertexID, Vertex vertex) {
    vertices.at(vertexID) = vertex;
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(vertexID * sizeof(Vertex)), sizeof(Vertex), &vertices[vertexID]);
}
*/
void TessellationHelper::Draw()
{
	Draw(0, 0, TrianglesCount);
}

void TessellationHelper::FreeMemory()
{
	Vertices.shrink_to_fit();
	TriangleIndices.shrink_to_fit();
}

void TessellationHelper::Draw(const size_t transformId, const size_t startPos, size_t count)
{
	if (Vertices.empty() && !HasInit)
	{
		return;
	}
	Shader::SetMat4(PositionUniform, GetTransformationMatrix(transformId));
	glBindVertexArray(Vao);
	if (!HasInit)
	{
		HasInit = true;
		glBindBuffer(GL_ARRAY_BUFFER, Vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLintptr>(Vertices.size() * sizeof(Vertex)), Vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, IndexTexture)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(TriangleIndices.size() * sizeof(GLuint)), TriangleIndices.data(), GL_STATIC_DRAW);
		TrianglesCount = TriangleIndices.size();
		Vertices.clear();
		TriangleIndices.clear();
	}
	if (count == 0)
	{
		count = TrianglesCount;
	}
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, reinterpret_cast<void*>(startPos * sizeof(GLuint)));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

size_t TessellationHelper::AddTransform(const Transform transform)
{
	TessellationTransforms.push_back(transform);
	return TessellationTransforms.size() - 1;
}

void TessellationHelper::Reset()
{
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);
    Vertices.clear();
    TriangleIndices.clear();
    HasInit = false;
}

TessellationHelper::TessellationHelper(Shader* shader, const float x, const float y, const float z) : TessellationHelper(shader)
{
	GetTransform(0).SetPosition(x, y, z);
}