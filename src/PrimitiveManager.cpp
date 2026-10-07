#include "PrimitiveManager.h"
#include "Modules/Render.h"

Primitive& PrimitiveManager::CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos)
{
    primitivesSize += 1;

    PrimitiveEntity entity;
    entity.type = type;
    entity.position = Pos;

    PrimitiveData* meshData;
    switch (type) {
    case PrimitiveType::ICOSAHEDRON:
        meshData = new Icosahedron();
        break;
    case PrimitiveType::CUBE:
        meshData = new Cube();
        break;
    case PrimitiveType::PYRAMID:
        meshData = new Pyramid();
        break;
    case PrimitiveType::CYLINDER:
        meshData = new Cylinder();
        break;
    case PrimitiveType::SPHERE:
        meshData = new Sphere();
        break;
    default:
        meshData = new Tetrahedron();
        break;
    }

    PrimitiveMesh mesh;
    mesh = Engine::GetInstance().render->CreateMesh(meshData->vertex.data(), meshData->vertex.size() * sizeof(GLfloat), meshData->index.data(), meshData->num_index);// TODO: get mesh data depending on the primitive type

    Primitive p = Primitive(mesh, entity);
    primitives.emplace(primitivesSize, p);

    return p;
}