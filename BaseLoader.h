#pragma once

#include "d3dUtil.h"
#include "Vertex.h"

class BaseLoader
{
public:

	const UINT MAX_VERTICES = 2000000;

	virtual bool Load(ID3D11Device* device, std::string filename, std::vector<Vertex::NormalTexVertex>& vertices, std::vector<UINT>& indices, 
		std::vector<MeshGeometry::Subset>& subsets, bool isRHS, bool uvFlipped, Effect *effect){ return true; }

	BaseLoader(){}
	~BaseLoader(){}
};

