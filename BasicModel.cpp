#include "BasicModel.h"
#include "OBJLoader.h"
#include "MazeLoader.h"

BasicModel::BasicModel(ID3D11Device* device, LitTexEffect* effect, std::string filename, bool isRHS, bool isVFlipped)

{
	mModelMesh = new MeshGeometry(effect);

	std::vector<ID3D11ShaderResourceView*> srvs;
	if (OBJLoader::Load(device, filename, mVertices, mIndices, mSubsets, isRHS, isVFlipped))
	{
		mModelMesh->SetVertices(device, &mVertices[0], mVertices.size());
		mModelMesh->SetIndices(device, &mIndices[0], mIndices.size());
		mModelMesh->SetSubsetTable(mSubsets);
	}
}

BasicModel::BasicModel(ID3D11Device* device, LitMatEffect* effect, std::string filename)

{
	mModelMesh = new BasicMeshGeometry(effect);

	std::vector<ID3D11ShaderResourceView*> srvs;
	if (MazeLoader::Load(device, filename, mVertices, mIndices))
	{
		mModelMesh->SetVertices(device, &mVertices[0], mVertices.size());
		mModelMesh->SetIndices(device, &mIndices[0], mIndices.size());
	}
}


BasicModel::~BasicModel(void)
{
	if(mModelMesh)
	{
		delete mModelMesh;
	}
}
