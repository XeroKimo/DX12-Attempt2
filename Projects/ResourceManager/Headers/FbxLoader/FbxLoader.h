#pragma once

//#include <fbxsdk.h>
#include "stdafx.h"


using namespace fbxsdk;
using namespace DirectX;

struct BoneIndexAndWeight
{
	BYTE mBoneIndices;
	float mBoneWeight;

	bool operator < (const BoneIndexAndWeight& rhs)
	{
		return (mBoneWeight > rhs.mBoneWeight);
	}
};

struct CtrlPoint
{
	DirectX::XMFLOAT3 mPosition;
	std::vector<BoneIndexAndWeight> mBoneInfo;
	std::string mBoneName;

	CtrlPoint()
	{
		mBoneInfo.reserve(4);
	}

	void SortBlendingInfoByWeight()
	{
		std::sort(mBoneInfo.begin(), mBoneInfo.end());
	}
};

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	
	HRESULT LoadFBX(
		std::vector<Vertex>& outVertexVector, 
		std::vector<uint32_t>& outIndexVector, 
		std::vector<Material>& outMaterial,
		std::string fileName);



	bool LoadMesh(
		std::string fileName,
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial = nullptr);


	void GetControlPoints(fbxsdk::FbxNode * pFbxRootNode);
	
	void GetVerticesAndIndice(
		fbxsdk::FbxMesh * pMesh,
		std::vector<Vertex>& outVertexVector, 
		std::vector<uint32_t>& outIndexVector);


	void GetMaterials(fbxsdk::FbxNode * pNode, std::vector<Material>& outMaterial);

	void GetMaterialAttribute(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& outMaterial);
	
	void GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& Mat);

	void ExportMesh(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName);
	
	void clear();

private:
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;

	inline void TokenizePathStr(std::string const& str, const char delim,
		std::vector<std::string>& out)
	{
		size_t start;
		size_t end = 0;

		while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			out.push_back(str.substr(start, end - start));
		}
	}

	inline std::string MatTextureToRelativePath(std::string filePathAndName, std::string matName)
	{
		std::string pathStr = filePathAndName;
		const char delim1 = '//';

		std::vector<std::string> outPath;
		TokenizePathStr(pathStr, delim1, outPath);
		std::string path;
		for (int i = 0; i < outPath.size() - 1; i++) {
			path += outPath[i] + "\\";
		}

		const char delim2 = '\\';
		std::vector<std::string> out;
		TokenizePathStr(matName, delim2, out);
		std::string matPathFile = path + out[out.size() - 1];

		return matPathFile;
	}

	inline bool MatTexFileExists(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

	void FixMissingMaterial(std::vector<Material>& outMaterial);
	void FixMaterialPathing(std::string path , std::vector<Material>& outMaterial);

};