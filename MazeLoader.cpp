#include "MazeLoader.h"
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
UINT MazeLoader::mMazeWidth;
UINT MazeLoader::mMazeHeight;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mWalls;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPellets;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPowerUps;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mPacMans;
std::vector<MazeLoader::MazeElementSpecs> MazeLoader::mGhosts;

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
	std::vector<Vertex::InstancedData>& instWalls, std::vector<Vertex::InstancedData>& instPellets, std::vector<Vertex::InstancedData>& instPowerUps,
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

	mElementCount.walls = 0;
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
		mElementCount.walls += std::count(wLine.begin(), wLine.end(), L'X');
		mElementCount.pellets += std::count(wLine.begin(), wLine.end(), L' ');
		mElementCount.powerUps += std::count(wLine.begin(), wLine.end(), L'O');
		mElementCount.emptySpaces += std::count(wLine.begin(), wLine.end(), L'-');

		mazeText.push_back(line);
	}

	mMazeHeight = lineCount;

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData meshBox;;
	GeometryGenerator::MeshData meshPellet;
	GeometryGenerator::MeshData meshPowerUp;
	GeometryGenerator::MeshData meshPacMan;
	GeometryGenerator::MeshData meshGhost;

	geoGen.CreateBox(0.70f, 0.20f, 0.70f, meshBox);
	geoGen.CreateSphere(RADIUS_PELLET, 10, 10, meshPellet);
	geoGen.CreateSphere(RADIUS_POWERUP, 10, 10, meshPowerUp);
	geoGen.CreateSphere(RADIUS_PAC_MAN, 10, 10, meshPacMan);
	geoGen.CreateSphere(RADIUS_GHOST, 10, 10, meshGhost);

	UINT startVertexPellet = meshBox.Vertices.size() * 1;
	UINT startIndexPellet = meshBox.Indices.size() * 1;
	UINT startVertexPowerUp = startVertexPellet + (meshPellet.Vertices.size() * 1);
	UINT startIndexPowerUp = startIndexPellet + (meshPellet.Indices.size() * 1);
	//UINT startVertexPowerUp = startVertexPellet + (meshPellet.Vertices.size() * mElementCount.pellets);
	//UINT startIndexPowerUp = startIndexPellet + (meshPellet.Indices.size() * mElementCount.pellets);
	UINT startVertexPacMan = startVertexPowerUp + (meshPowerUp.Vertices.size() * 1);
	UINT startIndexPacMan = startIndexPowerUp + (meshPowerUp.Indices.size() * 1);
	UINT startVertexGhost = startVertexPacMan + (meshPacMan.Vertices.size() * 1);
	UINT startIndexGhost = startIndexPacMan + (meshPacMan.Indices.size() * 1);
	
	UINT countVertex = (meshBox.Vertices.size() * 1) + (meshPellet.Vertices.size() * 1) + (meshPowerUp.Vertices.size() * 1)
		+ (meshPacMan.Vertices.size() * 1) + (meshGhost.Vertices.size() * 1);
	UINT countIndex = (meshBox.Indices.size() * mElementCount.walls) + (meshPellet.Indices.size() * 1) + (meshPowerUp.Indices.size() * mElementCount.powerUps) 
		+ (meshPacMan.Indices.size() * 1) + (meshGhost.Indices.size() * 1);

	vertices.resize(countVertex);
	indices.resize(countIndex);
	instWalls.resize(mElementCount.walls);
	instPellets.resize(mElementCount.pellets);
	instPowerUps.resize(mElementCount.powerUps);
	instPacMans.resize(3);
	instGhosts.resize(4);

	float posX = lineWidth * -0.5f;	// horizontal
	float posZ = lineCount * -0.5f;	// vertical
	UINT vCountWall = 0;
	UINT iCountWall = 0;
	UINT vCountPellet = startVertexPellet;
	UINT iCountPellet = startIndexPellet;
	UINT vCountPowerUp = startVertexPowerUp;
	UINT iCountPowerUp = startIndexPowerUp;
	UINT vCountPacMan = startVertexPacMan;
	UINT iCountPacMan = startIndexPacMan;
	UINT vCountGhost = startVertexGhost;
	UINT iCountGhost = startIndexGhost;
	UINT iBlockWall = 0;
	UINT iBlockPellet = startVertexPellet;
	UINT iBlockPowerUp = startVertexPowerUp;
	UINT iBlockPacMan = startVertexPacMan;
	UINT iBlockGhost = startVertexGhost;

	UINT instanceCountWall = 0;
	UINT instanceCountPellet = 0;
	UINT instanceCountPowerUp = 0;
	UINT instanceCountPacMan = 0;
	UINT instanceCountGhost = 0;

	mElementOffsetsCounts.walls.vertexOffset = 0;
	mElementOffsetsCounts.walls.indexOffset = 0;
	mElementOffsetsCounts.walls.vertexCount = meshBox.Vertices.size() * 1;
	mElementOffsetsCounts.walls.indexCount = meshBox.Indices.size() * 1;
	mElementOffsetsCounts.walls.vertexSize = meshBox.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshBox.Indices.size();

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

	for (size_t k = 0; k < meshBox.Vertices.size(); ++k, ++vCountWall)
	{
		vertices[vCountWall].pos.x = meshBox.Vertices[k].Position.x;
		vertices[vCountWall].pos.y = meshBox.Vertices[k].Position.y;
		vertices[vCountWall].pos.z = meshBox.Vertices[k].Position.z;
		vertices[vCountWall].normal = meshBox.Vertices[k].Normal;
		vertices[vCountWall].tex = meshBox.Vertices[k].TexC;
	}
	for (size_t k = 0; k < meshBox.Indices.size(); ++k, ++iCountWall)
	{
		indices[iCountWall] = meshBox.Indices[k] + iBlockWall;
	}
	iBlockWall += meshBox.Vertices.size();
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

	for (int i = 0; i < mazeText.size(); ++i)
	{
		for (int j = 0; j < mazeText[i].length(); ++j)
		{
			if (mazeText[i][j] == L'X')	// wall
			{
				mBoxData.push_back(AABox(XMVectorSet(posX, 0.0f, -posZ, 0.0f), 0.5f, 0.5f));
				mMazeElements.push_back(ME_WALL);
				worldPos._41 = posX;
				worldPos._42 = 0.0f;
				worldPos._43 = -posZ;
				mWalls.push_back(MazeElementSpecs(worldPos, Materials::BOX.Diffuse, true, true));
				instWalls[instanceCountWall].World = worldPos;
				instWalls[instanceCountWall++].Color = Materials::BOX.Diffuse;
			}
			if (mazeText[i][j] == L' ')	// pellets
			{
				mMazeElements.push_back(ME_PELLET);
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
			if (mazeText[i][j] == L'=')	// Really blank, but okay to travel to
			{
				mMazeElements.push_back(ME_NOTHING);
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

	return true;
}

bool MazeLoader::IsBlocked(UINT row, UINT col)
{
	UINT index = (row * mMazeWidth) + col;

	return ((mMazeElements[index] == ME_WALL) || (mMazeElements[index] == ME_BLANK));
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

bool MazeLoader::IsPellet(UINT row, UINT col)
{
	UINT index = (row * mMazeWidth) + col;

	return (mMazeElements[index] == ME_PELLET);
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

