//#include "PrimitiveManager.h"
//#include "Modules/Render.h"
//#include "../Editor/Engine.h"
//#include "TextureLoader.h"
//
//Primitive& PrimitiveManager::CreatePrimitiveEntity(PrimitiveType type, glm::vec3 Pos)
//{
//    primitivesSize += 1;
//
//    PrimitiveEntity entity;
//    entity.type = type;
//    entity.position = Pos;
//
//    PrimitiveData* meshData;
//    switch (type) {
//    case PrimitiveType::ICOSAHEDRON:
//        meshData = new Icosahedron();
//        break;
//    case PrimitiveType::CUBE:
//        meshData = new Cube();
//        break;
//    case PrimitiveType::PYRAMID:
//        meshData = new Pyramid();
//        break;
//    case PrimitiveType::CYLINDER:
//        meshData = new Cylinder();
//        break;
//    case PrimitiveType::SPHERE:
//        meshData = new Sphere();
//        break;
//    default:
//        meshData = new Tetrahedron();
//        break;
//    }
//
//    PrimitiveMesh mesh;
//    mesh = Engine::GetInstance().render->CreateMesh(meshData->vertex.data(), meshData->vertex.size() * sizeof(float), meshData->index.data(), meshData->num_index * sizeof(int));
//
//    Texture texture = TextureLoader::LoadTexture(meshData->texturePath);
//
//    Primitive p = Primitive(mesh, entity, texture);
//    primitives.emplace(primitivesSize, p);
//
//    delete(meshData);
//
//    return primitives[primitivesSize];
//}