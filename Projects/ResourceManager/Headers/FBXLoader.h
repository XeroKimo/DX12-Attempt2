#pragma once
#include <fbxsdk.h>
#include <vector>
#include <string>
#include <memory>

struct Vector2
{
    float x, y;
};
struct Vector3
{
    float x, y, z;
};

class Vertex
{
    Vector3 pos;
    Vector3 normal;
    Vector2 texCoord;
};

inline void LoadFBX(std::vector<Vertex>& outVertices, std::vector<unsigned int> outIndices, std::string fileName)
{
    std::string fbxFilename = fileName + ".fbx";
    FbxManager* manager = FbxManager::Create();
    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);


    FbxScene* scene = FbxScene::Create(manager, "");
    FbxImporter* importer = FbxImporter::Create(manager, "");
    importer->Initialize(fbxFilename.c_str(), -1, manager->GetIOSettings());
    importer->Import(scene);
    importer->Destroy();

    FbxGeometryConverter converter(manager);
    converter.Triangulate(scene, true);
    FbxNode* rootNode = scene->GetRootNode();
    if (rootNode)
    {
        for (int i = 0; i < rootNode->GetChildCount(); i++)
        {
            FbxNode* child = rootNode->GetChild(i);
            FbxNodeAttribute* attribute = child->GetNodeAttribute();
            if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
            {
                FbxMesh* mesh = reinterpret_cast<FbxMesh*>(attribute);

            }
        }
    }
}