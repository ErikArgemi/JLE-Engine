#include "PrimitiveManager.h"
#include "Modules/Render.h"

Primitive& PrimitiveManager::CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos)
{
    primitivesSize += 1;

    PrimitiveEntity entity;
    entity.type = type;
    entity.position = Pos;

    
    MeshContainer meshContainer;
    PrimitiveMeshData meshData = meshContainer.GetMesh(type);

    PrimitiveMesh mesh;
    mesh = Engine::GetInstance().render->CreateMesh(meshData.vertex.data(), meshData.vertex.size()*sizeof(GLfloat), meshData.index.data(), meshData.num_index);

    Primitive p = Primitive(mesh, entity);
    primitives.emplace(primitivesSize, p);

    return p;
}