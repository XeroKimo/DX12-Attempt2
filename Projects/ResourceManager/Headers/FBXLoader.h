#pragma once
#include "FBXLoaderInternal.h"
#include <fbxsdk.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

namespace FBXLoader
{
	inline FbxNode* LoadFBX(std::string fileName)
	{
		std::string fbxFilename = fileName + ".fbx";
		FbxManager* manager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(ioSettings);

		FbxScene* scene = FbxScene::Create(manager, "");
		FbxImporter* importer = FbxImporter::Create(manager, "");
		bool success = importer->Initialize(fbxFilename.c_str(), -1, manager->GetIOSettings());
		importer->Import(scene);
		importer->Destroy();

		FbxGeometryConverter converter(manager);
		converter.Triangulate(scene, true);
		FbxNode* rootNode = scene->GetRootNode();

		//manager->Destroy();
		return rootNode;
	}

	inline LoadMeshData LoadMesh(FbxNode* const rootNode)
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
		return meshData;
	}

}