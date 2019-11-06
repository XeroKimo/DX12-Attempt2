#pragma once
#include <fbxsdk.h>
#include <vector>
#include <string>

namespace FBXLoader
{
	struct LoadMeshMetaData
	{
	private:
		struct Vector3 { float x = 0, y = 0, z = 0; };
		struct Vector2{ float x = 0, y = 0; };
	public:
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;

		bool operator==(const LoadMeshMetaData& other) const
		{
			if (pos.x != other.pos.x || pos.y != other.pos.y || pos.z != other.pos.z)
				return false;

			if (normal.x != other.normal.x || normal.y != other.normal.y || normal.z != other.normal.z)
				return false;

			if (uv.x != other.uv.x || uv.y != other.uv.y)
				return false;

			return true;
		}

	};


	struct LoadMeshData
	{
		std::vector<LoadMeshMetaData> vertices;
		std::vector<unsigned int> indices;
	};

	namespace Internal
	{
		inline LoadMeshMetaData LoadVertex(FbxMesh* const mesh, unsigned int polygonIndex, unsigned int vertexIndex)
		{
			LoadMeshMetaData vertex;
			int index = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
			vertex.pos.x = mesh->GetControlPointAt(index).mData[0];
			vertex.pos.y = mesh->GetControlPointAt(index).mData[1];
			vertex.pos.z = mesh->GetControlPointAt(index).mData[2];

			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normal);
			vertex.normal.x = normal.mData[0];
			vertex.normal.y = normal.mData[1];
			vertex.normal.z = normal.mData[2];

			FbxStringList uvNames;
			const char* uvName = nullptr;
			mesh->GetUVSetNames(uvNames);
			if (uvNames.GetCount())
			{
				uvName = uvNames[0];
			}

			FbxVector2 uv;
			bool uvDefined;
			mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvName, uv, uvDefined);
			
			vertex.uv.x = uv.mData[0];
			vertex.uv.y = uv.mData[1];

			return vertex;
		}
	}
}

template<>
struct std::hash<FBXLoader::LoadMeshMetaData>
{
	size_t operator()(const FBXLoader::LoadMeshMetaData& key) const
	{
		return std::hash<float>()(key.pos.x) ^ std::hash<float>()(key.pos.y) ^ std::hash<float>()(key.pos.z)
			^ std::hash<float>()(key.normal.x) ^ std::hash<float>()(key.normal.y) ^ std::hash<float>()(key.normal.z)
			^ std::hash<float>()(key.uv.x) ^ std::hash<float>()(key.uv.y);
	}
};