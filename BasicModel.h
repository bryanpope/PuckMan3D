#pragma once

#include "d3dUtil.h"
#include "MeshGeometry.h"
#include "Vertex.h"
#include "BaseLoader.h"

class BasicModel
{
public:
	BasicModel() :
		mSubsetCount(0){}

	BasicModel(ID3D11Device* device, LitMatEffect* effect, std::string filename);

	virtual ~BasicModel(void);

	BasicMeshGeometry* GetMesh(){ return mModelMesh; }

protected:
	UINT mSubsetCount;

	std::vector<Vertex::NormalTexVertex> mVertices;

	std::vector<UINT> mIndices;

	std::vector<Vertex::InstancedData> mInstancesFloors;
	std::vector<Vertex::InstancedData> mInstancesWallsBent;
	std::vector<Vertex::InstancedData> mInstancesWallsStraight;
	std::vector<Vertex::InstancedData> mInstancesPellets;
	std::vector<Vertex::InstancedData> mInstancesPowerUps;
	std::vector<Vertex::InstancedData> mInstancesPacMans;
	std::vector<Vertex::InstancedData> mInstancesGhosts;
	std::vector<Vertex::InstancedData> mInstancesTriggers;
	std::vector<Vertex::InstancedData> mInstancesTraps;
	std::vector<MeshGeometry::Subset> mSubsets;

	BasicMeshGeometry* mModelMesh;
};

