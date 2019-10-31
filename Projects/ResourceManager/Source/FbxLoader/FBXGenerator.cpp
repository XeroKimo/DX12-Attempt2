//#include "Textures.h"
//#include "Materials.h"
//#include "FbxLoader.h"
//#include "FBXGenerator.h"

FBXGenerator::FBXGenerator()
	:mInBeginEndPair(false)
{
}


FBXGenerator::~FBXGenerator()
{
}

void FBXGenerator::Begin(ID3D12Device * device, ID3D12GraphicsCommandList * cmdList, ID3D12DescriptorHeap* cbvHeap)
{
	if (mInBeginEndPair)
		throw std::exception("Cannot nest Begin calls on a FBX Generator");
	//Set Variables
	mDevice = device;
	mCommandList = cmdList;
	mCbvHeap = cbvHeap;

	mInBeginEndPair = true;
}

void FBXGenerator::End()
{
	if (!mInBeginEndPair)
		throw std::exception("Begin must be called before End");

	mDevice = nullptr;
	mCommandList = nullptr;
	mCbvHeap = nullptr;

	mInBeginEndPair = false;
}

void FBXGenerator::LoadFBXStaticGeometry(
	std::string folderPath,
	std::string fbxModelName,
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& mGeometries, 
	Textures& mTexDiffuse, Textures& mTexturesNormal, Materials& mMaterials)
{
	//TODO Week 7: DECLARE a variable called fbxLoader of type FbxLoader
	FbxLoader fbxLoader;
	//TODO Week 7: DECLARE a variable called outMaterial of type std::vector<Material>
	std::vector<Material> outMaterial;
	//TODO Week 7: DECLARE a variable called FileName of type std::string
	std::string FileName;

	//TODO Week 7: DECLARE a variable called fbxVertex of type std::vector<Vertex>
	std::vector<Vertex> fbxVertex;
	//TODO Week 7: DECLARE a variable called fbxIndex of type std::vector<uint32_t>
	std::vector<uint32_t> fbxIndex;

	//TODO Week 7: ASSIGN folderPath + fbxModelName to FileName
	FileName = folderPath + fbxModelName; 
	//TODO Week 7: CALL LoadFBX(...) on the fbxLoader passing in --> fbxVertex, fbxIndex, outMaterial, FileName
	fbxLoader.LoadFBX(fbxVertex, fbxIndex, outMaterial, FileName);
	//TODO Week 7: CALL clear() on the fbxLoader
	fbxLoader.clear();

	//TODO Week 7: CALL BuildFBXGeometry(...) passing in --> fbxVertex, fbxIndex, fbxModelName, mGeometries
	BuildFBXGeometry(fbxVertex, fbxIndex, fbxModelName, mGeometries);
	//TODO Week 7: CALL BuildFBXTexture(...) passing in --> outMaterial, fbxModelName, mTexDiffuse, mTexturesNormal, mMaterials
	BuildFBXTexture(outMaterial, fbxModelName, mTexDiffuse, mTexturesNormal, mMaterials);

	//TODO Week 7: CALL clear() on fbxVertex
	fbxVertex.clear();
	//TODO Week 7: CALL clear() on fbxIndex
	fbxIndex.clear();
	//TODO Week 7: CALL clear() on outMaterial
	outMaterial.clear();
}

void FBXGenerator::BuildFBXGeometry(const std::vector<Vertex>& outVertices,
									  const std::vector<std::uint32_t>& outIndices, 
									  const std::string& geoName, 
									  std::unordered_map<std::string, 
									  std::unique_ptr<MeshGeometry>>& mGeometries)
{
	//TODO Week 7: DECLARE a variable called vertexOffset of type UINT and SET it to 0
	UINT vertexOffset = 0;
	//TODO Week 7: DECLARE a variable called indexOffset of type UINT and SET it to 0
	UINT indexOffset = 0;
	
	//TODO Week 7: DECLARE a variable called submesh of type SubmeshGeometry
	SubmeshGeometry submesh;

	// Submesh
	//TODO Week 7: DECLARE a variable called box of type DirectX::BoundingBox
	DirectX::BoundingBox box;
	//TODO Week 7: CALL DirectX::BoundingBox::CreateFromPoints(...) and pass in --> box,	outVertices.size(),	&outVertices[0].Pos, sizeof(Vertex)
	DirectX::BoundingBox::CreateFromPoints(
			box,
			outVertices.size(),
			&outVertices[0].Pos,
			sizeof(Vertex));

	//TODO Week 7: SET submesh.Bounds to box
	submesh.Bounds = box;
	//TODO Week 7: SET submesh.IndexCount to (UINT)outIndices.size()
	submesh.IndexCount = (UINT)outIndices.size();
	//TODO Week 7: SET submesh.StartIndexLocation to indexOffset
	submesh.StartIndexLocation = indexOffset;
	//TODO Week 7: SET submesh.BaseVertexLocation to vertexOffset
	submesh.BaseVertexLocation = vertexOffset;

	//TODO Week 7: ADD outVertices.size() to vertexOffset
	vertexOffset += outVertices.size();
	//TODO Week 7: ADD outIndices.size() to indexOffset
	indexOffset += outIndices.size();

	// vertex
	//TODO Week 7: DECLARE a variable called vertices of type std::vector<Vertex> and initialize it passing in vertexOffset
	std::vector<Vertex> vertices(vertexOffset);
	//TODO Week 7: DECLARE a variable called k of type UINT and SET it to 0
	UINT k = 0;
	//TODO Week 7: FOR int j = 0; j < outVertices.size(); ++j, ++k
	for (int j = 0; j < outVertices.size(); ++j, ++k)
	{
		//TODO Week 7: SET vertices[k].Pos to outVertices[j].Pos;
		vertices[k].Pos = outVertices[j].Pos;
		//TODO Week 7: SET vertices[k].Normal to outVertices[j].Normal;
		vertices[k].Normal = outVertices[j].Normal;
		//TODO Week 7: SET vertices[k].TexC to outVertices[j].TexC;
		vertices[k].TexC = outVertices[j].TexC;
	}
	//ENDFOR int j = 0; j < outVertices.size(); ++j, ++k

	// index
	//TODO Week 7: DECLARE  a variable called indices of type std::vector<std::uint32_t>
	std::vector<std::uint32_t> indices;

	//TODO Week 7: CALL insert(...) on indices passing in --> indices.end(), outIndices.begin(), outIndices.end()
	indices.insert(indices.end(), outIndices.begin(), outIndices.end());

	//Calculate vertex and buffer size
	//TODO Week 7: DECLARE a variable called vbByteSize of type const UINT vbByteSize and SET it to --> (UINT)vertices.size() * sizeof(Vertex)
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	//TODO Week 7: DECLARE a variable called ibByteSize of type const UINT vbByteSize and SET it to --> (UINT)indices.size() * sizeof(std::uint32_t)
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint32_t);

	//TODO Week 7: DECLARE a variable called geo of type auto and Initialize it to a unique_ptr of type MeshGeometry --> std::make_unique<MeshGeometry>()
	auto geo = std::make_unique<MeshGeometry>();
	//TODO Week 7: SET geo->Name to geoName
	geo->Name = geoName;

	//TODO Week 7: CALL D3DCreateBlob(..) passing in --> vbByteSize, &geo->VertexBufferCPU) , CALL it inside ThrowIfFailed(...)
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	//TODO Week 7: CALL CopyMemory(...) passing in --> geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	//TODO Week 7: CALL D3DCreateBlob(..) passing in --> ibByteSize, &geo->IndexBufferCPU , CALL it inside ThrowIfFailed(...)
	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	//TODO Week 7: CALL CopyMemory(...) passing in --> geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	//TODO Week 7: SET geo->VertexBufferGPU to the return value of d3dUtil::CreateDefaultBuffer(mDevice,	mCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader)
	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice,
		mCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	//TODO Week 7: SET geo->IndexBufferGPU to the return value of  d3dUtil::CreateDefaultBuffer(mDevice,	mCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader)
	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice,
		mCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	//TODO Week 7: SET geo->VertexByteStride  to sizeof(Vertex)
	geo->VertexByteStride = sizeof(Vertex);
	//TODO Week 7: SET geo->VertexBufferByteSize to vbByteSize
	geo->VertexBufferByteSize = vbByteSize;
	//TODO Week 7: SET geo->IndexFormat to DXGI_FORMAT_R32_UINT
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	//TODO Week 7: SET geo->IndexBufferByteSize to ibByteSize
	geo->IndexBufferByteSize = ibByteSize;

	//TODO Week 7: SET geo->DrawArgs[geoName] to submesh
	geo->DrawArgs[geoName] = submesh;

	//TODO Week 7: SET mGeometries[geo->Name] to std::move(geo) moving resources from geo into mGeometries[geo->Name]
	mGeometries[geo->Name] = std::move(geo);
}

void FBXGenerator::BuildFBXTexture(
	std::vector<Material>& outMaterial,
	std::string inTextureName,
	Textures& mTexDiffuse, Textures& mTexturesNormal, Materials& mMaterials)
{
	// Begin
	//TODO Week 7: CALL mTexDiffuse.Begin(...) passing in --> mDevice, mCommandList, mCbvHeap
	mTexDiffuse.Begin(mDevice, mCommandList, mCbvHeap);
	//TODO Week 7: CALL mTexturesNormal passing in --> mDevice, mCommandList, mCbvHeap
	mTexturesNormal.Begin(mDevice, mCommandList, mCbvHeap);

	// Load Texture and Material
	//TODO Week 7: DECLARE a variable called MatIndex and SET it to --> mMaterials.GetSize()
	int MatIndex = mMaterials.GetSize();
	//TODO Week 7: FOR int i = 0; i < outMaterial.size(); ++i
	for (int i = 0; i < outMaterial.size(); ++i)
	{
		//TODO Week 7: DECLARE a variable called TextureName of type std::string 
		std::string TextureName;
		// Load Texture 
		//TODO Week 7: IF !outMaterial[i].Name.empty()
		if (!outMaterial[i].Name.empty())
		{
			// Texture
			//TODO Week 7: SET TextureName to inTextureName
			TextureName = inTextureName;
			//TODO Week 7: ADD (i + 48) to TextureName --> push_back(i + 48) 48 is char '0'
			TextureName.push_back(i + 48);
			//TODO Week 7: DECLARE a variable called TextureFileName of type std::wstring
			std::wstring TextureFileName;
			//TODO Week 7: CALL assign(..) on TextureFileName passing in --> outMaterial[i].Name.begin(), outMaterial[i].Name.end()
			TextureFileName.assign(outMaterial[i].Name.begin(), outMaterial[i].Name.end());
			//TODO Week 7: CALL SetTexture(..) on mTexDiffuse passing in --> TextureName, TextureFileName
			mTexDiffuse.SetTexture(TextureName, TextureFileName);

			// Normal Map
			//TODO Week 7: DECLARE a variable called TextureNormalFileName of type std::wstring
			std::wstring TextureNormalFileName;
			//TODO Week 7: SET TextureNormalFileName to return value of --> TextureFileName.substr(0, TextureFileName.size() - 4)
			TextureNormalFileName = TextureFileName.substr(0, TextureFileName.size() - 4);
			//TODO Week 7: CALL append(.) on TextureNormalFileName passing in --> L"_normal.jpg"
			TextureNormalFileName.append(L"_normal.jpg");
			//TODO Week 7: DECLARE a variable called buffer of type struct stat --> http://codewiki.wikidot.com/c:struct-stat we only use it to check if the file is valid
			struct stat buffer;
			//TODO Week 7: DECLARE a variable called fileCheck of type std::string
			std::string fileCheck;
			//TODO Week 7: CALL fileCheck.assign(..) and pass in --> TextureNormalFileName.begin(), TextureNormalFileName.end()
			fileCheck.assign(TextureNormalFileName.begin(), TextureNormalFileName.end());
			//TODO Week 7: IF stat(fileCheck.c_str(), &buffer) == 0
			if (stat(fileCheck.c_str(), &buffer) == 0)
			{
				//TODO Week 7: CALL SetTexture(..) on mTexturesNormal passing in --> TextureName, TextureNormalFileName
				mTexturesNormal.SetTexture(TextureName, TextureNormalFileName);
			}
			//ENDIF stat(fileCheck.c_str(), &buffer) == 0
		}
		//ENDIF !outMaterial[i].Name.empty()

		// Load Material
		//TODO Week 7: DECLARE a variable called MaterialName and SET it to inTextureName
		std::string MaterialName = inTextureName;
		//TODO Week 7: ADD i + 48 to MaterialName --> push_back(i + 48)
		MaterialName.push_back(i + 48);

		//TODO Week 7: CALL SetMaterial(.......) on mMaterials passing in MaterialName, outMaterial[i].DiffuseAlbedo, outMaterial[i].FresnelR0, outMaterial[i].Roughness, MatIndex++, mTexDiffuse.GetTextureIndex(TextureName), mTexturesNormal.GetTextureIndex(TextureName)
		mMaterials.SetMaterial(
			MaterialName,
			outMaterial[i].DiffuseAlbedo,
			outMaterial[i].FresnelR0,
			outMaterial[i].Roughness,
			MatIndex++,
			mTexDiffuse.GetTextureIndex(TextureName),
			mTexturesNormal.GetTextureIndex(TextureName));
	}
	//ENDFOR int i = 0; i < outMaterial.size(); ++i
	//TODO Week 7: CALL End() on mTexDiffuse
	mTexDiffuse.End();
	//TODO Week 7: CALL End() on mTexturesNormal
	mTexturesNormal.End();
}