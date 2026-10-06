#include "PrimitiveManager.h"
#include "Render.h"

Primitive& PrimitiveManager::CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos)
{
    primitivesSize += 1;

    PrimitiveEntity entity;
    entity.type = type;
    entity.position = pos;

    PrimitiveMesh mesh;
    //meshData = GetPrimitiveData(type);
    //mesh = Engine::GetInstance().render->CreateMesh(mehData); TODO: get mesh data depending on the primitive type

    Primitive p = Primitive(mesh, entity);
    primitives.emplace(primitivesSize, p);

    return p;
}