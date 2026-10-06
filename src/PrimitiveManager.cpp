#include "PrimitiveManager.h"
#include "Render.h"

Primitive& PrimitiveManager::CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos)
{
    primitivesSize += 1;

    PrimitiveEntity entity;
    entity.type = type;
    entity.position = position;

    PrimitiveMesh mesh;
    //mesh = Engine::GetInstance().render->CreateMesh(); TODO: get mesh data depending on the primitive type

    Primitive p = Primitive(mesh, entity);
    primitives.emplace(primitivesSize, p);

    return p;
}