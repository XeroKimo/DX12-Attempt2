#pragma once
#include "FBXLoaderInternal.h"
#include <fbxsdk.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>

namespace FBXLoader
{
    extern FbxManager* gManager;
	inline FbxNode* LoadFBX(std::string fileName)
	{
		std::string fbxFilename = fileName + ".fbx";
        if (gManager == nullptr)
            gManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(gManager, IOSROOT);
        gManager->SetIOSettings(ioSettings);

		FbxScene* scene = FbxScene::Create(gManager, "");
		FbxImporter* importer = FbxImporter::Create(gManager, "");
		bool success = importer->Initialize(fbxFilename.c_str(), -1, gManager->GetIOSettings());
		importer->Import(scene);
		importer->Destroy();

		FbxGeometryConverter converter(gManager);
		converter.Triangulate(scene, true);
		FbxNode* rootNode = scene->GetRootNode();

        FbxAxisSystem axis = FbxAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
        axis.ConvertScene(scene, rootNode);


		//manager->Destroy();
		return rootNode;
	}

	inline LoadMeshData LoadMesh(std::string fileName, FbxNode* const rootNode)
	{
		LoadMeshData meshData;
		std::unordered_map<LoadMeshMetaData, unsigned int> loadedVertices;
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* child = rootNode->GetChild(i);
			FbxNodeAttribute* attribute = child->GetNodeAttribute();

			if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
			{
				FbxMesh* mesh = reinterpret_cast<FbxMesh*>(attribute); 
				for (unsigned int j = 0; j < mesh->GetPolygonCount(); j++)
				{
					for (unsigned int k = 0; k < 3; k++)
					{
						LoadMeshMetaData vertex = Internal::LoadVertex(mesh, j, k);
						auto it = loadedVertices.find(vertex);
						if (it != loadedVertices.end())
							meshData.indices.push_back(it->second);
						else
						{
							unsigned int index = static_cast<unsigned int>(meshData.vertices.size());

							meshData.vertices.push_back(vertex);
							meshData.indices.push_back(index);
							loadedVertices[vertex] = index;
						}
					}
				}
			}
		}

        std::ofstream fileOut(fileName + ".mesh");
        for (auto& e : meshData.vertices)
        {
            fileOut << "Pos " << e.pos.x << " " << e.pos.y << " " << e.pos.z << "\n";
            fileOut << "Normal " << e.normal.x << " " << e.normal.y << " " << e.normal.z << "\n";
            fileOut << "TexC " << e.uv.x << " " << e.uv.y << "\n";
        }
        fileOut << "Indices " << "\n";
        for (uint32_t i = 0; i < meshData.indices.size() / 3; ++i)
        {
            fileOut << meshData.indices[3 * i] << " " << meshData.indices[3 * i + 1] << " " << meshData.indices[3 * i + 2] << "\n";
        }
		return meshData;
	}

}