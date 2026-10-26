#include "Grid.h"
#include "../Components/MeshComponent.h"
#include "../../Editor/App.h"
#include "../Modules/Render.h"

Grid::Grid()
{
    meshComponent = std::make_unique<MeshComponent>();
    SetData(N);
}

Grid::~Grid()
{
    Clean();
}

void Grid::SetData(unsigned int size) {
    Clean();
    //vertex
    vertex.clear();
    for (int x = 0; x < size; ++x) {
        for (int z = 0; z < size; ++z) {
            vertex.emplace_back((x- (float)(size-1)/2) * unit);
            vertex.emplace_back(0);
            vertex.emplace_back((z -(float)(size-1)/2) * unit);

            vertex.emplace_back(1);
            vertex.emplace_back(1);
            vertex.emplace_back(1);

            vertex.emplace_back(0);
            vertex.emplace_back(0);
        }
    }
    
    //indices
    indices.clear();
    for (unsigned int i = 0; i < size * size; ++i) {
        unsigned int x = i / size;
        unsigned int z = i % size;

        if (z < size - 1) {
            indices.emplace_back(i);
            indices.emplace_back(i + 1);
        }
        if (x < size - 1) {
            indices.emplace_back(i);
            indices.emplace_back(i + size);
        }
    }

    Init();
}

void Grid::Init() {
    meshComponent->SetData(App::GetInstance().render->CreateMesh(vertex.data(), vertex.size() * sizeof(float), indices.data(), indices.size()));
}

void Grid::Render() {
    glBindVertexArray(meshComponent->mesh.m_VAO);
    glDrawElements(GL_LINES, meshComponent->n_index, GL_UNSIGNED_INT, 0);
}

void Grid::Clean() {
    auto& mesh = meshComponent->mesh;
    if (mesh.m_VAO != 0) glDeleteVertexArrays(1, &mesh.m_VAO);
    if (mesh.m_VBO != 0) glDeleteBuffers(1, &mesh.m_VBO);
    if (mesh.m_EBO != 0) glDeleteBuffers(1, &mesh.m_EBO);
}