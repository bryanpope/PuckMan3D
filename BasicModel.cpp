#include "BasicModel.h"
#include "OBJLoader.h"
#include "MazeLoader.h"

BasicModel::BasicModel(ID3D11Device* device, LitMatEffect* effect, std::string filename)

{
	mModelMesh = new BasicMeshGeometry(effect);

	std::vector<ID3D11ShaderResourceView*> srvs;
	if (MazeLoader::Load(device, filename, mVertices, mIndices, mInstancesFloors, mInstancesWallsBent, mInstancesWallsStraight, mInstancesPellets, mInstancesPowerUps, mInstancesPacMans, mInstancesGhosts, mInstancesTriggers, mInstancesTraps))
	{
		mModelMesh->SetVertices(device, &mVertices[0], mVertices.size());
		mModelMesh->SetIndices(device, &mIndices[0], mIndices.size());
		mModelMesh->SetInstancesFloor(device, &mInstancesFloors[0], mInstancesFloors.size());
		mModelMesh->SetInstancesWallsBent(device, &mInstancesWallsBent[0], mInstancesWallsBent.size());
		mModelMesh->SetInstancesWallsStraight(device, &mInstancesWallsStraight[0], mInstancesWallsStraight.size());
		mModelMesh->SetInstancesPellets(device, &mInstancesPellets[0], mInstancesPellets.size());
		mModelMesh->SetInstancesPowerUps(device, &mInstancesPowerUps[0], mInstancesPowerUps.size());
		mModelMesh->SetInstancesPacMans(device, &mInstancesPacMans[0], mInstancesPacMans.size());
		mModelMesh->SetInstancesGhosts(device, &mInstancesGhosts[0], mInstancesGhosts.size());
		mModelMesh->SetInstancesTriggers(device, &mInstancesTriggers[0], mInstancesTriggers.size());
		mModelMesh->SetInstancesTraps(device, &mInstancesTraps[0], mInstancesTraps.size());
	}
}


BasicModel::~BasicModel(void)
{
	if (mModelMesh)
	{
		delete mModelMesh;
	}
}



