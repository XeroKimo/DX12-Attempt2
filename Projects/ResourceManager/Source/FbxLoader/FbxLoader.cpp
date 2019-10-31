//#include <vector>
//#include <winerror.h>
//#include <assert.h>
//#include "FrameResource.h"
//#include "vertexHash.h"
#include "FbxLoader.h"



FbxManager* gFbxManager = nullptr;

FbxLoader::FbxLoader()
{
}

FbxLoader::~FbxLoader()
{	
}

HRESULT FbxLoader::LoadFBX(
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	//If exported fbx file exists
	if (LoadMesh(fileName, outVertexVector, outIndexVector, &outMaterial)) return S_OK;
	// if exported animation exist
	//if (LoadMesh(fileName, outVertexVector, outIndexVector)) return S_OK;

	//TODO Week 7: IF gFbxManager IS nullptr
	if (gFbxManager == nullptr)
	{
		//Create the fbxManager
		//TODO Week 7: ASSIGN gFbxManager to the return value of FbxManager::Create()
		gFbxManager = FbxManager::Create();

		//TODO Week 7: Create FbxIO settings
		//TODO Week 7: DECLARE a variable called pIOsettings of type FbxIOSettings* and assign it to the retuirn value of FbxIOSettings::Create(gFbxManager, IOSROOT)
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT);
		//TODO Week 7: CALL SetIOSettings(..) on the gFbxManager passing in pIOsettings
		gFbxManager->SetIOSettings(pIOsettings);
	}
	//ENDIF

	//Create the Fbx Importer using the FbxManager
	//TODO Week 7: DECLARE a variable called pImporter of type FbxImporter* and assign it to the return value of FbxImporter::Create(gFbxManager, "")
	FbxImporter* pImporter = FbxImporter::Create(gFbxManager, "");
	//TODO Week 7: DECLARE a variable called fbxFileName of type std::string and assign it to --> fileName + ".fbx"
	std::string fbxFileName = fileName + ".fbx";
	
	//Open the Fbx file
	//TODO Week 7: DECLARE a variable called bSuccess and assign it to the retuen value of --> pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings())
	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings());
	//TODO Week 7: IF bSuccess IS False
	if (!bSuccess) 
		return E_FAIL;
		//RETURN E_FAIL
	//ENDIF

	//Import the Fbx Scene
	//TODO Week 7: DECLARE a variable called pFbxScene of type FbxScene* and assign it to the return value of FbxScene::Create(gFbxManager, "")
	FbxScene* pFbxScene = FbxScene::Create(gFbxManager, "");
	//Import the scene
	//TODO Week 7: ASSIGN bSuccess to the return value of pImporter->Import(pFbxScene)
	bSuccess = pImporter->Import(pFbxScene);
	//TODO Week 7: IF bSucess IS false
	if (!bSuccess)
		return E_FAIL;
		//TODO Week 7: RETURN E_FAIL
	//ENDIF

	//We are done loading our fbx model/scene
	//TODO Week 7: CALL Destroy on the pImporter
	pImporter->Destroy();

	//FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	//FbxAxisSystem::MayaZUp.ConvertScene(pFbxScene); // Delete?

	// Convert quad to triangle
	//TODO Week 7: DECLARE a variable called geometryConverter of type FbxGeometryConverter and initialize it by passing in gFbxManager
	FbxGeometryConverter geometryConverter(gFbxManager);
	//TODO Week 7: CALL Triangulate(..) on the geometryConverter and pass in --> pFbxScene, true
	geometryConverter.Triangulate(pFbxScene, true);

	// Start reading from the RootNode
	//TODO Week 7: DECLARE a variable called pFbxRootNode of type FbxNode* and assign it to the return value of pFbxScene->GetRootNode()
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	//TODO Week 7: IF pFbxRootNode is NOT nullptr
	if (pFbxRootNode)
	{
		// Bone offset, Control point, Vertex, Index Data

		//int ii = pFbxRootNode->GetChildCount();
		//TODO Week 7: FOR int i = 0; i < pFbxRootNode->GetChildCount(); i++
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			//TODO Week 7: DECLARE a variable called pFbxChildNode of type FbxNode* and assign it to the return value of pFbxRootNode->GetChild(i)
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i); 
			//TODO Week 7: DECLARE a variable called pMesh of type FbxMesh* and assign it to the return value of (FbxMesh*)pFbxChildNode->GetNodeAttribute()
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			//TODO Week 7: IF pMesh IS nullptr
				//TODO Week 7: CONTINUE
			//ENDIF pMesh IS nullptr
			if (!pMesh) { continue; }

			//TODO Week 7: DECLARE a variable called AttributeType of type FbxNodeAttribute::EType and assign it to the return value of pMesh->GetAttributeType()
			FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();

			//TODO Week 7: IF NOT AttributeType
				//TODO Week 7: CONTINUE
			//ENDIF NOT AttributeType
			if (!AttributeType) { continue; }

			//TODO Week 7: IF AttributeType IS EQUAL to FbxNodeAttribute::eMesh
			if (AttributeType == FbxNodeAttribute::eMesh)
			{
				//Get control Points
				//TODO Week 7: CALL GetControlPoints(..) passing in pFbxChildNode
				GetControlPoints(pFbxChildNode);

				// Get Vertices and indices info
				//TODO Week 7: CALL GetVerticesAndIndice(..) passing in pMesh, outVertexVector, outIndexVector
				GetVerticesAndIndice(pMesh, outVertexVector, outIndexVector);

				//TODO Week 7: CALL GetMaterials(..) passing in pFbxChildNode, outMaterial
				GetMaterials(pFbxChildNode, outMaterial);

				//TODO Week 7: If there is a material, Check if material file exists in the given path, If Not then fix it with the texture in the current directory. The texture should be in the current directory
				FixMaterialPathing(fileName, outMaterial);

				//TODO Week 7:Check for Missing material and give it a default material
				FixMissingMaterial(outMaterial);

				break;
			}
			//ENDIF AttributeType IS EQUAL to FbxNodeAttribute::eMesh
		}
		//ENDFOR int i = 0; i < pFbxRootNode->GetChildCount(); i++
	}
	//ENDIF pFbxRootNode is NOT nullptr

	//TODO Week 7: CALL ExportMesh(...) passing in --> outVertexVector, outIndexVector, outMaterial, fileName (Go into this function and have a liik at what it does)
	ExportMesh(outVertexVector, outIndexVector, outMaterial, fileName);

	return S_OK;
}

bool FbxLoader::LoadMesh(
	std::string fileName,
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>* outMaterial)
{
	fileName = fileName + ".mesh";
	std::ifstream fileIn(fileName);

	uint32_t vertexSize, indexSize;
	uint32_t materialSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> vertexSize;
		fileIn >> ignore >> indexSize;
		fileIn >> ignore >> materialSize;

		if (vertexSize == 0 || indexSize == 0)
			return false;

		// Material Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < materialSize; ++i)
		{
			Material tempMaterial;

			fileIn >> ignore >> tempMaterial.Name;
			fileIn >> ignore >> tempMaterial.Ambient.x >> tempMaterial.Ambient.y >> tempMaterial.Ambient.z;
			fileIn >> ignore >> tempMaterial.DiffuseAlbedo.x >> tempMaterial.DiffuseAlbedo.y >> tempMaterial.DiffuseAlbedo.z >> tempMaterial.DiffuseAlbedo.w;
			fileIn >> ignore >> tempMaterial.FresnelR0.x >> tempMaterial.FresnelR0.y >> tempMaterial.FresnelR0.z;
			fileIn >> ignore >> tempMaterial.Specular.x >> tempMaterial.Specular.y >> tempMaterial.Specular.z;
			fileIn >> ignore >> tempMaterial.Emissive.x >> tempMaterial.Emissive.y >> tempMaterial.Emissive.z;
			fileIn >> ignore >> tempMaterial.Roughness;
			fileIn >> ignore;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileIn >> tempMaterial.MatTransform.m[i][j];
				}
			}
			(*outMaterial).push_back(tempMaterial);
		}

		// Vertex Data
		for (uint32_t i = 0; i < vertexSize; ++i)
		{
			Vertex vertex;
			fileIn >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
			fileIn >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
			fileIn >> ignore >> vertex.TexC.x >> vertex.TexC.y;

			// push_back
			outVertexVector.push_back(vertex);
		}

		// Index Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < indexSize; ++i)
		{
			uint32_t index;
			fileIn >> index;
			outIndexVector.push_back(index);
		}

		return true;
	}

	return false;
}

void FbxLoader::GetControlPoints(FbxNode * pFbxRootNode)
{
	//TODO Week 7: DECLARE a variable called pCurrMesh of type FbxMesh* and assign it to the return value of --> (FbxMesh*)pFbxRootNode->GetNodeAttribute()
	FbxMesh* pCurrMesh = (FbxMesh*)pFbxRootNode->GetNodeAttribute();

	//TODO Week 7: DECLARE a variable called ctrlPointCount of type unsigned int and assign it to the return value of pCurrMesh->GetControlPointsCount()
	unsigned int ctrlPointCount = pCurrMesh->GetControlPointsCount();

	//TODO Week 7: FOR unsigned int i = 0; i < ctrlPointCount; ++i
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		//TODO Week 7: DECLARE a variable called currCtrlPoint of type CtrlPoint* and assign it to the return value of --> new CtrlPoint()
		CtrlPoint* currCtrlPoint = new CtrlPoint();

		//TODO Week 7: DECLARE a variable called currPosition of type DirectX::XMFLOAT3
		DirectX::XMFLOAT3 currPosition;
		//SET x,y,z of currPosition, Uncomment code below 
		currPosition.x = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[2]);

		//TODO Week 7: SET currCtrlPoint->mPosition to currPosition
		currCtrlPoint->mPosition = currPosition;
		//TODO Week 7: SET  mControlPoints[i] to currCtrlPoint 
		mControlPoints[i] = currCtrlPoint;
	}
	//ENDFOR unsigned int i = 0; i < ctrlPointCount; ++i
}

void FbxLoader::GetVerticesAndIndice(
	FbxMesh* pMesh,
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector)
{
	// Vertex and Index
	//TODO Week 7: DECLARE a variable called IndexMapping of type std::unordered_map<Vertex, uint32_t>
	std::unordered_map<Vertex, uint32_t> IndexMapping;
	//TODO Week 7: DECALRE a variable called VertexIndex of type uint32_t and set it to ZERO
	uint32_t VertexIndex = 0;
	//TODO Week 7: DECLARE a variable called tCount of type int and assign it to the return value of pMesh->GetPolygonCount()
	int tCount = pMesh->GetPolygonCount(); // Triangle

	//TODO Week 7: FOR int i = 0; i < tCount; ++i
	for (int i = 0; i < tCount; ++i)
	{
		// Vertex and Index info
		//TODO Week 7: FOR int j = 0; j < 3; ++j
		for (int j = 0; j < 3; ++j)
		{
			//TODO Week 7: DECLARE a variable called controlPointIndex and assign it to the return value of --> pMesh->GetPolygonVertex(i, j)
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			//TODO Week 7: DECLARE a variable called CurrCtrlPoint and assign it to --> mControlPoints[controlPointIndex]
			CtrlPoint* CurrCtrlPoint = mControlPoints[controlPointIndex];

			// Normal
			//TODO Week 7: DECLARE a variable called pNormal of type FbxVector4
			FbxVector4 pNormal;
			//TODO Week 7: CALL GetPolygonVertexNormal(...) on pMesh and pass in --> i, j, pNormal
			pMesh->GetPolygonVertexNormal(i, j, pNormal);

			// UV
			//TODO Week 7: DECLARE a variable called lUVs of type float* and assign NULL to it
			float* lUVs = NULL;
			//TODO Week 7: DECLARE a variable called lUVNames of type FbxStringList
			FbxStringList lUVNames;
			//TODO Week 7: CALL GetUVSetNames(..) on pMesh passing in lUVNames
			pMesh->GetUVSetNames(lUVNames);
			//TODO Week 7: DECLARE a variable called lUVName of type const char* and assign NULL to it
			const char* lUVName = NULL;
			//TODO Week 7: IF lUVNames.GetCount()
			if (lUVNames.GetCount())
			{
				//TODO Week 7: SET lUVName to lUVNames[0]
				lUVName = lUVNames[0];
			}
			//END lUVNames.GetCount()

			//TODO Week 7: DECLARE a variable called pUVs of type FbxVector2
			FbxVector2 pUVs;
			//TODO Week 7: DECLARE a variable called bUnMappedUV of type bool
			bool bUnMappedUV;

			//TODO Week 7: IF !pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV)
			if (!pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV))
			{
				MessageBox(0, L"UV not found", 0, 0);
			}
			//ENDIF !pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV)

			//TODO Week 7: DECLARE a variable called Temp of type Vertex
			Vertex Temp;
			// Position
			//TODO Week 7: SET Temp.Pos.x, Temp.Pos.y, Temp.Pos.z //UNCOMMENT LINES BELOW
			Temp.Pos.x = CurrCtrlPoint->mPosition.x;
			Temp.Pos.y = CurrCtrlPoint->mPosition.y;
			Temp.Pos.z = CurrCtrlPoint->mPosition.z;

			// Normal
			//TODO Week 7: SET Temp.Normal.x, Temp.Normal.y, Temp.Normal.z //UNCOMMENT LINES BELOW
			Temp.Normal.x = pNormal.mData[0];
			Temp.Normal.y = pNormal.mData[1];
			Temp.Normal.z = pNormal.mData[2];

			// UV
			//TODO Week 7: SET Temp.TexC.x, Temp.TexC.y //UNCOMMENT LINES BELOW
			Temp.TexC.x = pUVs.mData[0];
			Temp.TexC.y = 1.0f - pUVs.mData[1];

			// push vertex and index
			//TODO Week 7: DECLARE a auto variable called lookup and assign it to the return value of --> IndexMapping.find(Temp)
			auto lookup = IndexMapping.find(Temp);
			//TODO Week 7: IF lookup != IndexMapping.end()
			if (lookup != IndexMapping.end())
			{
				// Index
				//TODO Week 7: ADD lookup->second to the outIndexVector vector
				outIndexVector.push_back(lookup->second);
			}
			//ELSE
			else
			{
				// Index
				//TODO Week 7: ADD VertexIndex to the outIndexVector vector
				outIndexVector.push_back(VertexIndex);
				//TODO Week 7: SET IndexMapping[Temp] to VertexIndex
				IndexMapping[Temp] = VertexIndex;

				//TODO Week 7: INCREMENT VertexIndex
				VertexIndex++;
				//TODO Week 7: ADD Temp to the outVertexVector vector
				outVertexVector.push_back(Temp);
			}
			//ENDIF lookup != IndexMapping.end()
		}
		//ENDFOR int j = 0; j < 3; ++j
	}
	//ENDIF int i = 0; i < tCount; ++i
}

void FbxLoader::GetMaterials(FbxNode* pNode, std::vector<Material>& outMaterial)
{
	//TODO Week 7: DECLARE a variable called MaterialCount of type int and assign it to the return value of pNode->GetMaterialCount()
	int MaterialCount = pNode->GetMaterialCount();

	//TODO Week 7: FOR int i = 0; i < MaterialCount; ++i
	for (int i = 0; i < MaterialCount; ++i)
	{
		//TODO Week 7: DECLARE a variable called tempMaterial of type Material
		Material tempMaterial;
		//TODO Week 7: DECLARE a variable called SurfaceMaterial of type FbxSurfaceMaterial* and assign it to the return value of pNode->GetMaterial(i)
		FbxSurfaceMaterial* SurfaceMaterial = pNode->GetMaterial(i);
		//TODO Week 7: CALL GetMaterialAttribute(...) and pass in --> SurfaceMaterial, tempMaterial (Go into this Function and see what it does)
		GetMaterialAttribute(SurfaceMaterial, tempMaterial);
		//TODO Week 7: CALL GetMaterialTexture(...) and pass in --> SurfaceMaterial, tempMaterial (Go into this Function and see what it does, Its messy, It can be optimized)
		GetMaterialTexture(SurfaceMaterial, tempMaterial);

		//TODO Week 7: IF tempMaterial.Name != ""
		if (tempMaterial.Name != "")
		{
			//TODO Week 7: ADD tempMaterial to outMaterial vector
			outMaterial.push_back(tempMaterial);
		}
		//ENDIF tempMaterial.Name != ""
	}
	//ENDFOR int i = 0; i < MaterialCount; ++i
}

void FbxLoader::GetMaterialAttribute(FbxSurfaceMaterial* pMaterial, Material& outMaterial)
{
	FbxDouble3 double3;
	FbxDouble double1;
	if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Ambient;
		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Diffuse;
		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);

		// Roughness 
		double1 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Shininess;
		outMaterial.Roughness = 1 - double1;

		// Reflection
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Reflection;
		outMaterial.FresnelR0.x = static_cast<float>(double3.mData[0]);
		outMaterial.FresnelR0.y = static_cast<float>(double3.mData[1]);
		outMaterial.FresnelR0.z = static_cast<float>(double3.mData[2]);

		// Specular Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Specular;
		outMaterial.Specular.x = static_cast<float>(double3.mData[0]);
		outMaterial.Specular.y = static_cast<float>(double3.mData[1]);
		outMaterial.Specular.z = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Emissive;
		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);

		/*
		// Transparency Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->TransparencyFactor;
		currMaterial->mTransparencyFactor = double1;

		// Specular Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->SpecularFactor;
		currMaterial->mSpecularPower = double1;


		// Reflection Factor
	double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->ReflectionFactor;
		currMaterial->mReflectionFactor = double1;	*/
	}
	else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Ambient;
		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Diffuse;
		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Emissive;
		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);
	}
}

void FbxLoader::GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& Mat)
{
	unsigned int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		//Get the property
		property = pMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		//If property is valid
		if (property.IsValid())
		{
			//Get the texture count 
			unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
			//iterate textures
			for (unsigned int i = 0; i < textureCount; ++i)
			{
				//Get the layered Texture
				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (layeredTexture)
				{
					throw std::exception("Layered Texture is currently unsupported\n");
				}
				else //otherwise its a texture
				{
					//Get the texture
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						//Get the texture type
						std::string textureType = property.GetNameAsCStr();
						//Get the texture file
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							if (textureType == "DiffuseColor")
							{
								//Set the material name to texture files name
								Mat.Name = fileTexture->GetFileName();
							}
						}
					}
				}
			}
		}
	}
}

void FbxLoader::ExportMesh(
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	std::ofstream fileOut(fileName + ".mesh");

	if (outVertexVector.empty() || outIndexVector.empty())
		return;

	if (fileOut)
	{
		uint32_t vertexSize = outVertexVector.size();
		uint32_t indexSize = outIndexVector.size();
		uint32_t materialSize = outMaterial.size();

		fileOut << "VertexSize " << vertexSize << "\n";
		fileOut << "IndexSize " << indexSize << "\n";
		fileOut << "MaterialSize " << materialSize << "\n";

		fileOut << "Material " << "\n";
		for (auto& e : outMaterial)
		{
			fileOut << "Name " << e.Name << "\n";
			fileOut << "Ambient " << e.Ambient.x << " " << e.Ambient.y << " " << e.Ambient.z << "\n";
			fileOut << "Diffuse " << e.DiffuseAlbedo.x << " " << e.DiffuseAlbedo.y << " " << e.DiffuseAlbedo.z << " " << e.DiffuseAlbedo.w << "\n";
			fileOut << "Fresnel " << e.FresnelR0.x << " " << e.FresnelR0.y << " " << e.FresnelR0.z << "\n";
			fileOut << "Specular " << e.Specular.x << " " << e.Specular.y << " " << e.Specular.z << "\n";
			fileOut << "Emissive " << e.Emissive.x << " " << e.Emissive.y << " " << e.Emissive.z << "\n";
			fileOut << "Roughness " << e.Roughness << "\n";
			fileOut << "MatTransform ";
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileOut << e.MatTransform.m[i][j] << " ";
				}
			}
			fileOut << "\n";
		}

		for (auto& e : outVertexVector)
		{
			fileOut << "Pos " << e.Pos.x << " " << e.Pos.y << " " << e.Pos.z << "\n";
			fileOut << "Normal " << e.Normal.x << " " << e.Normal.y << " " << e.Normal.z << "\n";
			fileOut << "TexC " << e.TexC.x << " " << e.TexC.y << "\n";
		}

		fileOut << "Indices " << "\n";
		for (uint32_t i = 0; i < indexSize / 3; ++i)
		{
			fileOut << outIndexVector[3 * i] << " " << outIndexVector[3 * i + 1] << " " << outIndexVector[3 * i + 2] << "\n";
		}
	}
}

void FbxLoader::clear()
{
	mControlPoints.clear();
}

void FbxLoader::FixMissingMaterial(std::vector<Material>& outMaterial)
{

	if (!outMaterial.size())
	{
		Material tempMaterial;
		tempMaterial.Ambient.x = 0;
		tempMaterial.Ambient.y = 0;
		tempMaterial.Ambient.z = 0;

		// Diffuse Color					
		tempMaterial.DiffuseAlbedo.x = 0.8f;
		tempMaterial.DiffuseAlbedo.y = 0.8f;
		tempMaterial.DiffuseAlbedo.z = 0.8f;
		tempMaterial.DiffuseAlbedo.w = 1;

		// Roughness 					
		tempMaterial.Roughness = -8.60784f;

		// Reflection
		tempMaterial.FresnelR0.x = 1;
		tempMaterial.FresnelR0.y = 1;
		tempMaterial.FresnelR0.z = 1;

		// Specular Color					
		tempMaterial.Specular.x = 1;
		tempMaterial.Specular.y = 1;
		tempMaterial.Specular.z = 1;

		// Emissive Color					
		tempMaterial.Emissive.x = 0.8f;
		tempMaterial.Emissive.y = 0.8f;
		tempMaterial.Emissive.z = 0.8f;

		tempMaterial.MatTransform = MathHelper::Identity4x4();

		tempMaterial.Name = "..\\Resource\\Textures\\red.png";

		outMaterial.push_back(tempMaterial);
	}
}

void FbxLoader::FixMaterialPathing(std::string path, std::vector<Material>& outMaterial)
{
	for (int i = 0; i < outMaterial.size(); i++)
	{
		if (!MatTexFileExists(outMaterial[i].Name))
		{
			outMaterial[i].Name = MatTextureToRelativePath(path, outMaterial[i].Name);
		}
	}
}
