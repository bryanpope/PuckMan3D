#include "MazeLoader.h"
#include "OBJLoader.h"
#include "GeometryGenerator.h"
#include "Materials.h"
#include <fstream>
#include <algorithm>

const UINT MazeLoader::MAX_VERTICES = 2000000;
const UINT MazeLoader::MAX_LINE_WIDTH = 256;
MazeLoader::MazeElementsCounts MazeLoader::mElementCount;
MazeLoader::OffsetsCountsMazeElements MazeLoader::mElementOffsetsCounts;
MazeLoader::InitialPosition MazeLoader::mInitialPositions;
std::vector<MazeLoader::AABox> MazeLoader::mBoxData;
std::vector<MazeLoader::MazeElements> MazeLoader::mMazeElements;
std::vector<MazeLoader::MazeElements> MazeLoader::mMazeElementsModify;
UINT MazeLoader::mMazeWidth;
UINT MazeLoader::mMazeHeight;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mFloor;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mWallsBent;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mWallsStraight;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPellets;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPowerUps;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPacMans;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mGhosts;

std::vector<Vertex::NormalTexVertex> MazeLoader::mCylVerticesStraight;
std::vector<UINT> MazeLoader::mCylIndicesStraight;
std::vector<MeshGeometry::Subset> MazeLoader::mCylSubsetsStraight;
std::vector<Vertex::NormalTexVertex> MazeLoader::mCylVerticesBent;
std::vector<UINT> MazeLoader::mCylIndicesBent;
std::vector<MeshGeometry::Subset> MazeLoader::mCylSubsetsBent;

const float MazeLoader::RADIUS_PAC_MAN = 0.75f;
const float MazeLoader::RADIUS_GHOST = 0.75f;
const float MazeLoader::RADIUS_PELLET = 0.125f;
const float MazeLoader::RADIUS_POWERUP = 0.25f;

MazeLoader::MazeLoader(void)
{
}

MazeLoader::~MazeLoader(void)
{
}

bool MazeLoader::Load(ID3D11Device* device, std::string filename, std::vector<Vertex::NormalTexVertex>& vertices, std::vector<UINT>& indices, 
	std::vector<Vertex::InstancedData>& instFloor,
	std::vector<Vertex::InstancedData>& instWallsBent, std::vector<Vertex::InstancedData>& instWallsStraight,
	std::vector<Vertex::InstancedData>& instPellets, std::vector<Vertex::InstancedData>& instPowerUps,
	std::vector<Vertex::InstancedData>& instPacMans, std::vector<Vertex::InstancedData>& instGhosts)
{
	std::vector<XMFLOAT3> vertPos;
	vertPos.reserve(MAX_VERTICES);

	std::vector<XMFLOAT3> normals;
	vertPos.reserve(MAX_VERTICES);

	std::vector<XMFLOAT2> uvs;
	uvs.reserve(MAX_VERTICES);

	std::wifstream fin(filename.c_str());

	std::vector<std::wstring> mazeText;
	XMFLOAT4X4 worldPos;
	XMStoreFloat4x4(&worldPos, XMMatrixIdentity());

	if(!fin)
	{
		OutputDebugString(L"Error loading mesh file.");
		return false;
	}

	if (!OBJLoader::Load(device, "Models/bentCylinder2.obj", mCylVerticesBent, mCylIndicesBent, mCylSubsetsBent, false, false))
	{
		OutputDebugString(L"Error loading cylinderBent.obj");
		return false;
	}
	if (!OBJLoader::Load(device, "Models/cylinderStraight.obj", mCylVerticesStraight, mCylIndicesStraight, mCylSubsetsStraight, false, false))
	{
		OutputDebugString(L"Error loading cylinderStraight.obj");
		return false;
	}

	mElementCount.floors = 1;
	mElementCount.walls.cornerTopLeft = 0;
	mElementCount.walls.cornerTopRight = 0;
	mElementCount.walls.cornerBottomRight = 0;
	mElementCount.walls.cornerBottomLeft = 0;
	mElementCount.walls.vertical = 0;
	mElementCount.walls.horizontal = 0;
	mElementCount.pellets = 0;
	mElementCount.powerUps = 0;
	mElementCount.emptySpaces = 0;

	wchar_t line[256];
	UINT lineCount = 0;
	UINT lineWidth = 0;
	while (fin.getline(line, MAX_LINE_WIDTH))
	{
		++lineCount;
		if (lineWidth == 0)
		{
			lineWidth = fin.gcount() - 1;
			mMazeWidth = lineWidth;
		}
		/*else
		{
			int readLineWidth = fin.gcount();
			if (readLineWidth != lineWidth)
			{
				OutputDebugString(L"Maze line widths should be all the same!");
				return false;
			}
		}*/
		std::wstring wLine = line;
		mElementCount.walls.cornerTopLeft += std::count(wLine.begin(), wLine.end(), L'0');
		mElementCount.walls.cornerTopRight += std::count(wLine.begin(), wLine.end(), L'1');
		mElementCount.walls.cornerBottomRight += std::count(wLine.begin(), wLine.end(), L'2');
		mElementCount.walls.cornerBottomLeft += std::count(wLine.begin(), wLine.end(), L'3');
		mElementCount.walls.vertical += std::count(wLine.begin(), wLine.end(), L'4');
		mElementCount.walls.horizontal += std::count(wLine.begin(), wLine.end(), L'5');
		mElementCount.walls.horizontal += std::count(wLine.begin(), wLine.end(), L'6');
		mElementCount.pellets += std::count(wLine.begin(), wLine.end(), L' ');
		mElementCount.pellets += std::count(wLine.begin(), wLine.end(), L'D');
		mElementCount.powerUps += std::count(wLine.begin(), wLine.end(), L'O');
		mElementCount.emptySpaces += std::count(wLine.begin(), wLine.end(), L'-');

		mazeText.push_back(line);
	}

	mMazeHeight = lineCount;

	GeometryGenerator geoGen;
	//GeometryGenerator::MeshData meshBox;
	GeometryGenerator::MeshData meshFloor;
	GeometryGenerator::MeshData meshPellet;
	GeometryGenerator::MeshData meshPowerUp;
	GeometryGenerator::MeshData meshPacMan;
	GeometryGenerator::MeshData meshGhost;

	//geoGen.CreateBox(0.70f, 0.20f, 0.70f, meshBox);
	geoGen.CreateGrid(mMazeWidth * 3, mMazeHeight * 3, mMazeHeight, mMazeWidth, meshFloor);
	//geoGen.CreateBox(mMazeWidth, 1.0f, mMazeHeight, meshFloor);
	geoGen.CreateSphere(RADIUS_PELLET, 10, 10, meshPellet);
	geoGen.CreateSphere(RADIUS_POWERUP, 10, 10, meshPowerUp);
	geoGen.CreateSphere(RADIUS_PAC_MAN, 10, 10, meshPacMan);
	geoGen.CreateSphere(RADIUS_GHOST, 10, 10, meshGhost);

	UINT startVertexWallBent = meshFloor.Vertices.size() * 1;
	UINT startIndexWallBent = meshFloor.Indices.size() * 1;
	UINT startVertexWallStraight = startVertexWallBent + mCylVerticesBent.size() * 1;
	UINT startIndexWallStraight = startIndexWallBent + mCylIndicesBent.size() * 1;
	UINT startVertexPellet = startVertexWallStraight + mCylVerticesStraight.size() * 1;
	UINT startIndexPellet = startIndexWallStraight + mCylIndicesStraight.size() * 1;
	UINT startVertexPowerUp = startVertexPellet + (meshPellet.Vertices.size() * 1);
	UINT startIndexPowerUp = startIndexPellet + (meshPellet.Indices.size() * 1);
	//UINT startVertexPowerUp = startVertexPellet + (meshPellet.Vertices.size() * mElementCount.pellets);
	//UINT startIndexPowerUp = startIndexPellet + (meshPellet.Indices.size() * mElementCount.pellets);
	UINT startVertexPacMan = startVertexPowerUp + (meshPowerUp.Vertices.size() * 1);
	UINT startIndexPacMan = startIndexPowerUp + (meshPowerUp.Indices.size() * 1);
	UINT startVertexGhost = startVertexPacMan + (meshPacMan.Vertices.size() * 1);
	UINT startIndexGhost = startIndexPacMan + (meshPacMan.Indices.size() * 1);
	
	UINT countVertex = (meshFloor.Vertices.size() * 1) + (mCylVerticesBent.size() * 1) + (mCylVerticesStraight.size() * 1) + (meshPellet.Vertices.size() * 1) + (meshPowerUp.Vertices.size() * 1)
		+ (meshPacMan.Vertices.size() * 1) + (meshGhost.Vertices.size() * 1);
	UINT countIndex = (meshFloor.Indices.size() * 1) + (mCylIndicesBent.size() * 1) + (mCylIndicesStraight.size() * 1) + (meshPellet.Indices.size() * 1) + (meshPowerUp.Indices.size() * 1)
		+ (meshPacMan.Indices.size() * 1) + (meshGhost.Indices.size() * 1);

	UINT countWallsBent = mElementCount.walls.cornerTopLeft + mElementCount.walls.cornerTopRight + mElementCount.walls.cornerBottomRight + mElementCount.walls.cornerBottomLeft;
	UINT countWallsStraight = mElementCount.walls.vertical + mElementCount.walls.horizontal;

	vertices.resize(countVertex);
	indices.resize(countIndex);
	instFloor.resize(mElementCount.floors);
	instWallsBent.resize(countWallsBent);
	instWallsStraight.resize(countWallsStraight);
	instPellets.resize(mElementCount.pellets);
	instPowerUps.resize(mElementCount.powerUps);
	instPacMans.resize(3);
	instGhosts.resize(4);

	float posX = lineWidth * -0.5f;	// horizontal
	float posZ = lineCount * -0.5f;	// vertical
	UINT vCountFloor = 0;
	UINT iCountFloor = 0;
	UINT vCountWallBent = startVertexWallBent;
	UINT iCountWallBent = startIndexWallBent;
	UINT vCountWallStraight = startVertexWallStraight;
	UINT iCountWallStraight = startIndexWallStraight;
	UINT vCountPellet = startVertexPellet;
	UINT iCountPellet = startIndexPellet;
	UINT vCountPowerUp = startVertexPowerUp;
	UINT iCountPowerUp = startIndexPowerUp;
	UINT vCountPacMan = startVertexPacMan;
	UINT iCountPacMan = startIndexPacMan;
	UINT vCountGhost = startVertexGhost;
	UINT iCountGhost = startIndexGhost;
	UINT iBlockFloor = 0;
	UINT iBlockWallBent = startVertexWallBent;
	UINT iBlockWallStraight = startVertexWallStraight;
	UINT iBlockPellet = startVertexPellet;
	UINT iBlockPowerUp = startVertexPowerUp;
	UINT iBlockPacMan = startVertexPacMan;
	UINT iBlockGhost = startVertexGhost;

	UINT instanceCountFloor = 0;
	UINT instanceCountWallBent = 0;
	UINT instanceCountWallStraight = 0;
	UINT instanceCountPellet = 0;
	UINT instanceCountPowerUp = 0;
	UINT instanceCountPacMan = 0;
	UINT instanceCountGhost = 0;

	mElementOffsetsCounts.floors.vertexOffset = 0;
	mElementOffsetsCounts.floors.indexOffset = 0;
	mElementOffsetsCounts.floors.vertexCount = meshFloor.Vertices.size() * 1;
	mElementOffsetsCounts.floors.indexCount = meshFloor.Indices.size() * 1;
	mElementOffsetsCounts.floors.vertexSize = meshFloor.Vertices.size();
	mElementOffsetsCounts.floors.indexSize = meshFloor.Indices.size();

	mElementOffsetsCounts.wallsBent.vertexOffset = startVertexWallBent;
	mElementOffsetsCounts.wallsBent.indexOffset = startIndexWallBent;
	mElementOffsetsCounts.wallsBent.vertexCount = mCylVerticesBent.size() * 1;
	mElementOffsetsCounts.wallsBent.indexCount = mCylIndicesBent.size() * 1;
	mElementOffsetsCounts.wallsBent.vertexSize = mCylVerticesBent.size();
	mElementOffsetsCounts.wallsBent.indexSize = mCylIndicesBent.size();

	mElementOffsetsCounts.wallsStraight.vertexOffset = startVertexWallStraight;
	mElementOffsetsCounts.wallsStraight.indexOffset = startIndexWallStraight;
	mElementOffsetsCounts.wallsStraight.vertexCount = mCylVerticesStraight.size() * 1;
	mElementOffsetsCounts.wallsStraight.indexCount = mCylIndicesStraight.size() * 1;
	mElementOffsetsCounts.wallsStraight.vertexSize = mCylVerticesStraight.size();
	mElementOffsetsCounts.wallsStraight.indexSize = mCylIndicesStraight.size();

	mElementOffsetsCounts.pellets.vertexOffset = startVertexPellet;
	mElementOffsetsCounts.pellets.indexOffset = startIndexPellet;
	mElementOffsetsCounts.pellets.vertexCount = meshPellet.Vertices.size() * 1;
	mElementOffsetsCounts.pellets.indexCount = meshPellet.Indices.size() * 1;
	mElementOffsetsCounts.pellets.vertexSize = meshPellet.Vertices.size();
	mElementOffsetsCounts.pellets.indexSize = meshPellet.Indices.size();

	mElementOffsetsCounts.powerUps.vertexOffset = startVertexPowerUp;
	mElementOffsetsCounts.powerUps.indexOffset = startIndexPowerUp;
	mElementOffsetsCounts.powerUps.vertexCount = meshPowerUp.Vertices.size() * 1;
	mElementOffsetsCounts.powerUps.indexCount = meshPowerUp.Indices.size() * 1;
	mElementOffsetsCounts.powerUps.vertexSize = meshPowerUp.Vertices.size();
	mElementOffsetsCounts.powerUps.indexSize = meshPowerUp.Indices.size();

	mElementOffsetsCounts.pacMan.vertexOffset = startVertexPacMan;
	mElementOffsetsCounts.pacMan.indexOffset = startIndexPacMan;
	mElementOffsetsCounts.pacMan.vertexCount = meshPacMan.Vertices.size() * 1;
	mElementOffsetsCounts.pacMan.indexCount = meshPacMan.Indices.size() * 1;
	mElementOffsetsCounts.pacMan.vertexSize = meshPacMan.Vertices.size();
	mElementOffsetsCounts.pacMan.indexSize = meshPacMan.Indices.size();

	mElementOffsetsCounts.ghosts.vertexOffset = startVertexGhost;
	mElementOffsetsCounts.ghosts.indexOffset = startIndexGhost;
	mElementOffsetsCounts.ghosts.vertexCount = meshGhost.Vertices.size() * 1;
	mElementOffsetsCounts.ghosts.indexCount = meshGhost.Indices.size() * 1;
	mElementOffsetsCounts.ghosts.vertexSize = meshGhost.Vertices.size();
	mElementOffsetsCounts.ghosts.indexSize = meshGhost.Indices.size();

	for (size_t k = 0; k < meshFloor.Vertices.size(); ++k, ++vCountFloor)
	{
		vertices[vCountFloor].pos.x = meshFloor.Vertices[k].Position.x;
		vertices[vCountFloor].pos.y = meshFloor.Vertices[k].Position.y;
		vertices[vCountFloor].pos.z = meshFloor.Vertices[k].Position.z;
		vertices[vCountFloor].normal = meshFloor.Vertices[k].Normal;
		vertices[vCountFloor].tex = meshFloor.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshFloor.Indices.size(); ++k, ++iCountFloor)
	{
		indices[iCountFloor] = meshFloor.Indices[k] + iBlockFloor;
	}
	iBlockFloor += meshFloor.Vertices.size();
	for (size_t k = 0; k < mCylVerticesBent.size(); ++k, ++vCountWallBent)
	{
		vertices[vCountWallBent].pos.x = mCylVerticesBent[k].pos.x;
		vertices[vCountWallBent].pos.y = mCylVerticesBent[k].pos.y;
		vertices[vCountWallBent].pos.z = mCylVerticesBent[k].pos.z;
		vertices[vCountWallBent].normal = mCylVerticesBent[k].normal;
		vertices[vCountWallBent].tex = mCylVerticesBent[k].tex;
	}
	for (size_t k = 0; k < mCylIndicesBent.size(); ++k, ++iCountWallBent)
	{
		indices[iCountWallBent] = mCylIndicesBent[k] + iBlockWallBent;
	}
	for (size_t k = 0; k < mCylVerticesStraight.size(); ++k, ++vCountWallStraight)
	{
		vertices[vCountWallStraight].pos.x = mCylVerticesStraight[k].pos.x;
		vertices[vCountWallStraight].pos.y = mCylVerticesStraight[k].pos.y;
		vertices[vCountWallStraight].pos.z = mCylVerticesStraight[k].pos.z;
		vertices[vCountWallStraight].normal = mCylVerticesStraight[k].normal;
		vertices[vCountWallStraight].tex = mCylVerticesStraight[k].tex;
	}
	for (size_t k = 0; k < mCylIndicesStraight.size(); ++k, ++iCountWallStraight)
	{
		indices[iCountWallStraight] = mCylIndicesStraight[k] + iBlockWallStraight;
	}
	iBlockWallStraight += mCylVerticesStraight.size();
	for (size_t k = 0; k < meshPellet.Vertices.size(); ++k, ++vCountPellet)
	{
		vertices[vCountPellet].pos.x = meshPellet.Vertices[k].Position.x;
		vertices[vCountPellet].pos.y = meshPellet.Vertices[k].Position.y;
		vertices[vCountPellet].pos.z = meshPellet.Vertices[k].Position.z;
		vertices[vCountPellet].normal = meshPellet.Vertices[k].Normal;
		vertices[vCountPellet].tex = meshPellet.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshPellet.Indices.size(); ++k, ++iCountPellet)
	{
		indices[iCountPellet] = meshPellet.Indices[k] + iBlockPellet;
	}
	iBlockPellet += meshPellet.Vertices.size();
	for (size_t k = 0; k < meshPowerUp.Vertices.size(); ++k, ++vCountPowerUp)
	{
		vertices[vCountPowerUp].pos.x = meshPowerUp.Vertices[k].Position.x;
		vertices[vCountPowerUp].pos.y = meshPowerUp.Vertices[k].Position.y;
		vertices[vCountPowerUp].pos.z = meshPowerUp.Vertices[k].Position.z;
		vertices[vCountPowerUp].normal = meshPowerUp.Vertices[k].Normal;
		vertices[vCountPowerUp].tex = meshPowerUp.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshPowerUp.Indices.size(); ++k, ++iCountPowerUp)
	{
		indices[iCountPowerUp] = meshPowerUp.Indices[k] + iBlockPowerUp;
	}
	iBlockPowerUp += meshPowerUp.Vertices.size();
	for (size_t k = 0; k < meshPacMan.Vertices.size(); ++k, ++vCountPacMan)
	{
		vertices[vCountPacMan].pos.x = meshPacMan.Vertices[k].Position.x;
		vertices[vCountPacMan].pos.y = meshPacMan.Vertices[k].Position.y;
		vertices[vCountPacMan].pos.z = meshPacMan.Vertices[k].Position.z;
		vertices[vCountPacMan].normal = meshPacMan.Vertices[k].Normal;
		vertices[vCountPacMan].tex = meshPacMan.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshPacMan.Indices.size(); ++k, ++iCountPacMan)
	{
		indices[iCountPacMan] = meshPacMan.Indices[k] + iBlockPacMan;
	}
	iBlockPacMan += meshPacMan.Vertices.size();
	for (size_t k = 0; k < meshGhost.Vertices.size(); ++k, ++vCountGhost)
	{
		vertices[vCountGhost].pos.x = meshGhost.Vertices[k].Position.x;
		vertices[vCountGhost].pos.y = meshGhost.Vertices[k].Position.y;
		vertices[vCountGhost].pos.z = meshGhost.Vertices[k].Position.z;
		vertices[vCountGhost].normal = meshGhost.Vertices[k].Normal;
		vertices[vCountGhost].tex = meshGhost.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshGhost.Indices.size(); ++k, ++iCountGhost)
	{
		indices[iCountGhost] = meshGhost.Indices[k] + iBlockGhost;
	}
	iBlockGhost += meshGhost.Vertices.size();

	for (int i = 0; i < 3; ++i)
	{
		instPacMans[i].World = worldPos;
		instPacMans[i].Color = XMFLOAT4(1.0f, 0.50f, 0.25f, 1.0f);
	}
	for (int i = 0; i < 4; ++i)
	{
		instGhosts[i].World = worldPos;
		instGhosts[i].Color = XMFLOAT4(1.0f, 0.50f, 0.25f, 1.0f);
	}

	worldPos._41 = 0.0f;
	worldPos._42 = -0.75f;
	worldPos._43 = 0.0f;
	mFloor.push_back(MazeElementSpecs(worldPos, Materials::GRID.Diffuse, true, true));
	instFloor[instanceCountFloor].World = worldPos;
	instFloor[instanceCountFloor++].Color = Materials::GRID.Diffuse;

	for (int i = 0; i < mazeText.size(); ++i)
	{
		for (int j = 0; j < mazeText[i].length(); ++j)
		{
			if (mazeText[i][j] >= L'0' && mazeText[i][j] <= L'6')	// wall
			{
				XMStoreFloat4x4(&worldPos, XMMatrixIdentity());
				switch (mazeText[i][j])
				{
				case L'0':		// Top Left Corner
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsBent[instanceCountWallBent].World = worldPos;
					instWallsBent[instanceCountWallBent++].Color = Materials::BOX.Diffuse;
					mWallsBent.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'1':		// Top Right Corner
					XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsBent[instanceCountWallBent].World = worldPos;
					instWallsBent[instanceCountWallBent++].Color = Materials::BOX.Diffuse;
					mWallsBent.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'2':		// Bottom Right Corner
					XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PI), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsBent[instanceCountWallBent].World = worldPos;
					instWallsBent[instanceCountWallBent++].Color = Materials::BOX.Diffuse;
					mWallsBent.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'3':		// Bottom Left Corner
					XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY((XM_PI + XM_PIDIV2)), XMLoadFloat4x4(&worldPos)));
					//XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsBent[instanceCountWallBent].World = worldPos;
					instWallsBent[instanceCountWallBent++].Color = Materials::BOX.Diffuse;
					mWallsBent.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'4':		// Vertical
					//XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PI), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsStraight[instanceCountWallStraight].World = worldPos;
					instWallsStraight[instanceCountWallStraight++].Color = Materials::BOX.Diffuse;
					mWallsStraight.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'5':		// Horizontal
					XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsStraight[instanceCountWallStraight].World = worldPos;
					instWallsStraight[instanceCountWallStraight++].Color = Materials::BOX.Diffuse;
					mWallsStraight.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
					mMazeElements.push_back(ME_WALL);
					break;
				case L'6':		// Horizontal, Ghost Gate
					XMStoreFloat4x4(&worldPos, XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMLoadFloat4x4(&worldPos)));
					worldPos._41 = posX;
					worldPos._42 = 0.0f;
					worldPos._43 = -posZ;
					instWallsStraight[instanceCountWallStraight].World = worldPos;
					instWallsStraight[instanceCountWallStraight++].Color = Materials::PINKY.Diffuse;
					mWallsStraight.push_back(MazeElementSpecs(worldPos, Materials::PINKY.Diffuse, true, true));
					mMazeElements.push_back(ME_NOTHING);
					break;
				}
				mBoxData.push_back(AABox(XMVectorSet(posX, 0.0f, -posZ, 0.0f), 0.5f, 0.5f));
			}
			if (mazeText[i][j] == L' ' || mazeText[i][j] == L'D')	// pellets or divergent pellets
			{
				mMazeElements.push_back(mazeText[i][j] == L' ' ? ME_PELLET : ME_PELLET_DIVERGENT);
				worldPos._41 = posX;
				worldPos._42 = 0.0f;
				worldPos._43 = -posZ;
				mPellets.push_back(MazeElementSpecs(worldPos, Materials::PELLET.Diffuse, true, true));
				instPellets[instanceCountPellet].World = worldPos;
				instPellets[instanceCountPellet++].Color = Materials::PELLET.Diffuse;
			}
			if (mazeText[i][j] == L'O')	// power up
			{
				mMazeElements.push_back(ME_POWERUP);
				worldPos._41 = posX;
				worldPos._42 = 0.0f;
				worldPos._43 = -posZ;
				mPowerUps.push_back(MazeElementSpecs(worldPos, Materials::POWERUP.Diffuse, true, true));
				instPowerUps[instanceCountPowerUp].World = worldPos;
				instPowerUps[instanceCountPowerUp++].Color = Materials::POWERUP.Diffuse;
			}
			if (mazeText[i][j] == L'M')	// Puck Man
			{
				mMazeElements.push_back(ME_NOTHING);
				worldPos._41 = posX + 0.5f;
				worldPos._42 = 0.0f;
				worldPos._43 = -posZ;
				mInitialPositions.pacMan = XMFLOAT3(worldPos._41, worldPos._42, worldPos._43);
				mPacMans.push_back(MazeElementSpecs(worldPos, Materials::PACMAN.Diffuse, true, true));
				instPacMans[instanceCountPacMan].World = worldPos;
				instPacMans[instanceCountPacMan++].Color = Materials::PACMAN.Diffuse;
			}
			if (mazeText[i][j] == L'B' || mazeText[i][j] == L'I' || mazeText[i][j] == L'P' || mazeText[i][j] == L'C')	// Ghosts
			{
				XMFLOAT4 colour;
				switch (mazeText[i][j])
				{
				case L'B':
					mInitialPositions.blinky = XMFLOAT3(posX + 0.5f, 0.0f, -posZ);
					colour = Materials::BLINKY.Diffuse;
					break;
				case L'I':
					mInitialPositions.inky = XMFLOAT3(posX + 0.5f, 0.0f, -posZ);
					colour = Materials::INKY.Diffuse;
					break;
				case L'P':
					mInitialPositions.pinky = XMFLOAT3(posX + 0.5f, 0.0f, -posZ);
					colour = Materials::PINKY.Diffuse;
					break;
				case L'C':
					mInitialPositions.clyde = XMFLOAT3(posX + 0.5f, 0.0f, -posZ);
					colour = Materials::CLYDE.Diffuse;
					break;
				}
				mMazeElements.push_back(ME_NOTHING);
				worldPos._41 = posX + 0.5f;
				worldPos._42 = 0.0f;
				worldPos._43 = -posZ;
				mGhosts.push_back(MazeElementSpecs(worldPos, colour, true, true));
				instGhosts[instanceCountGhost].World = worldPos;
				instGhosts[instanceCountGhost++].Color = colour;
			}
			if (mazeText[i][j] == L'=' || mazeText[i][j] == L'd')	// Really blank, but okay to travel to, maybe divergent
			{
				mMazeElements.push_back(mazeText[i][j] == L'=' ? ME_NOTHING : ME_NOTHING_DIVERGENT);
			}
			if (mazeText[i][j] == L'-')	// Really blank, don't go here
			{
				mMazeElements.push_back(ME_BLANK);
			}
			++posX;
			//iBlock += iCount;
		}
		posX = lineWidth * -0.5f;
		++posZ;
	}

	mPacMans.push_back(MazeElementSpecs(worldPos, Materials::PACMAN.Diffuse, true, true));
	mPacMans.push_back(MazeElementSpecs(worldPos, Materials::PACMAN.Diffuse, true, true));
	mMazeElementsModify = mMazeElements;

	return true;
}

bool MazeLoader::IsBlocked(UINT row, UINT col)
{
	if ((row >= mMazeHeight) || (col >= mMazeWidth))
	{
		return true;
	}

	UINT index = (row * mMazeWidth) + col;

	return ((mMazeElements[index] == ME_WALL) || (mMazeElements[index] == ME_BLANK));
}

bool MazeLoader::IsDivergent(UINT row, UINT col)
{
	if ((row >= mMazeHeight) || (col >= mMazeWidth))
	{
		return false;
	}

	UINT index = (row * mMazeWidth) + col;

	return ((mMazeElements[index] == ME_PELLET_DIVERGENT) || (mMazeElements[index] == ME_NOTHING_DIVERGENT));
}

void MazeLoader::SetPacManPos(FXMVECTOR pos, UINT index)
{ 
	mPacMans[index].pos.x = pos.m128_f32[0];
	mPacMans[index].pos.y = pos.m128_f32[1];
	mPacMans[index].pos.z = pos.m128_f32[2];

	mPacMans[index].world._41 = pos.m128_f32[0];
	mPacMans[index].world._42 = pos.m128_f32[1];
	mPacMans[index].world._43 = pos.m128_f32[2];
}

void MazeLoader::SetPacManVel(FXMVECTOR vel, UINT index)
{ 
	mPacMans[index].vel.x = vel.m128_f32[0];
	mPacMans[index].vel.y = vel.m128_f32[1];
	mPacMans[index].vel.z = vel.m128_f32[2];
}

void MazeLoader::SetGhostPos(FXMVECTOR pos, UINT index)
{ 
	mGhosts[index].pos.x = pos.m128_f32[0];
	mGhosts[index].pos.y = pos.m128_f32[1];
	mGhosts[index].pos.z = pos.m128_f32[2];

	mGhosts[index].world._41 = pos.m128_f32[0];
	mGhosts[index].world._42 = pos.m128_f32[1];
	mGhosts[index].world._43 = pos.m128_f32[2];
}

void MazeLoader::SetGhostVel(FXMVECTOR vel, UINT index)
{ 
	mGhosts[index].vel.x = vel.m128_f32[0];
	mGhosts[index].vel.y = vel.m128_f32[1];
	mGhosts[index].vel.z = vel.m128_f32[2];
}

void MazeLoader::SetGhostColour(XMFLOAT4 col, UINT index)
{
	mGhosts[index].colour = col;
}

void MazeLoader::RemovePellet(UINT index)
{ 
	mPellets[index].isShown = mPellets[index].isCollider = false;

	UINT row = mMazeHeight - (UINT)floor(mPellets[index].pos.z + (mMazeHeight * 0.5));
	UINT col = (UINT)floor(mPellets[index].pos.x + (mMazeWidth * 0.5));
	UINT mazeIndex = (row * mMazeWidth) + col;
	mMazeElementsModify[mazeIndex] = ME_NOTHING;
}

void MazeLoader::RemovePowerUp(UINT index)
{
	mPowerUps[index].isShown = mPowerUps[index].isCollider = false;

	UINT row = mMazeHeight - (UINT)floor(mPowerUps[index].pos.z + (mMazeHeight * 0.5));
	UINT col = (UINT)floor(mPowerUps[index].pos.x + (mMazeWidth * 0.5));
	UINT mazeIndex = (row * mMazeWidth) + col;
	mMazeElementsModify[mazeIndex] = ME_NOTHING;
}

void MazeLoader::RemoveLastPacMan()
{
	for (int i = mPacMans.size() - 1; i >= 0; --i)
	{
		if (mPacMans[i].isShown)
		{
			mPacMans[i].isShown = mPacMans[i].isCollider = false;
			break;
		}
	}
}

void MazeLoader::ResetPellets()
{
	for (int i = 0; i < mPellets.size(); ++i)
	{
		mPellets[i].isShown = mPellets[i].isCollider = true;
	}
	mMazeElementsModify = mMazeElements;
}

void MazeLoader::ResetPowerUps()
{
	for (int i = 0; i < mPowerUps.size(); ++i)
	{
		mPowerUps[i].isShown = mPowerUps[i].isCollider = true;
	}
	mMazeElementsModify = mMazeElements;
}

void MazeLoader::ResetPacMan()
{
	for (int i = 0; i < mPacMans.size(); ++i)
	{
		mPacMans[i].isShown = mPacMans[i].isCollider = true;
	}
	ResetPacManPosition();
}

void MazeLoader::ResetPacManPosition()
{
	mPacMans[0].pos.x = mInitialPositions.pacMan.x;
	mPacMans[0].pos.y = mInitialPositions.pacMan.y;
	mPacMans[0].pos.z = mInitialPositions.pacMan.z;
}

bool MazeLoader::IsPellet(UINT row, UINT col)
{
	UINT index = (row * mMazeWidth) + col;

	if ((row >= mMazeHeight) || (col >= mMazeWidth))
	{
		return false;
	}

	return (mMazeElementsModify[index] == ME_PELLET);
}


/*UINT MazeLoader::AddVertex(Vertex::NormalTexVertex vertex, std::vector<Vertex::NormalTexVertex>& vertBuf)
{
	HashEntry* currEntry = hashTable[hashValue];
	HashEntry* prevEntry = NULL;
	while(currEntry != NULL)
	{
		if(vertex.pos == vertBuf[currEntry->index].pos && 
			vertex.tex == vertBuf[currEntry->index].tex && 
			vertex.normal == vertBuf[currEntry->index].normal)
		{
			return currEntry->index;
		}
		prevEntry = currEntry;
		currEntry = currEntry->pNext;
	}

	currEntry = new HashEntry();
	currEntry->index = vertBuf.size(); 
	currEntry->pNext = NULL;

	if(prevEntry != NULL)
	{
		prevEntry->pNext = currEntry;
	}
	else
	{
		hashTable[hashValue] = currEntry;
	}

	vertBuf.push_back(vertex);
	
	return currEntry->index;
}*/

