#include "MazeLoader.h"
#include "GeometryGenerator.h"
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

bool MazeLoader::Load(ID3D11Device* device, std::string filename, std::vector<Vertex::NormalTexVertex>& vertices, std::vector<UINT>& indices)
{
	std::vector<XMFLOAT3> vertPos;
	vertPos.reserve(MAX_VERTICES);

	std::vector<XMFLOAT3> normals;
	vertPos.reserve(MAX_VERTICES);

	std::vector<XMFLOAT2> uvs;
	uvs.reserve(MAX_VERTICES);

	std::wifstream fin(filename.c_str());

	std::vector<std::wstring> mazeText;

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

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData meshBox;;
	GeometryGenerator::MeshData meshPellet;
	GeometryGenerator::MeshData meshPowerUp;
	GeometryGenerator::MeshData meshPacMan;
	GeometryGenerator::MeshData meshGhost;

	geoGen.CreateBox(0.5f, 0.5f, 0.5f, meshBox);
	geoGen.CreateSphere(RADIUS_PELLET, 10, 10, meshPellet);
	geoGen.CreateSphere(RADIUS_POWERUP, 10, 10, meshPowerUp);
	geoGen.CreateSphere(RADIUS_PAC_MAN, 10, 10, meshPacMan);
	geoGen.CreateSphere(RADIUS_GHOST, 10, 10, meshGhost);

	UINT startVertexPellet = meshBox.Vertices.size() * mElementCount.walls;
	UINT startIndexPellet = meshBox.Indices.size() * mElementCount.walls;
	UINT startVertexPowerUp = startVertexPellet + (meshPellet.Vertices.size() * mElementCount.pellets);
	UINT startIndexPowerUp = startIndexPellet + (meshPellet.Indices.size() * mElementCount.pellets);
	UINT startVertexPacMan = startVertexPowerUp + (meshPowerUp.Vertices.size() * mElementCount.powerUps);
	UINT startIndexPacMan = startIndexPowerUp + (meshPowerUp.Indices.size() * mElementCount.powerUps);
	UINT startVertexGhost = startVertexPacMan + (meshPacMan.Vertices.size() * 1);
	UINT startIndexGhost = startIndexPacMan + (meshPacMan.Indices.size() * 1);
	
	UINT countVertex = (meshBox.Vertices.size() * mElementCount.walls) + (meshPellet.Vertices.size() * mElementCount.pellets) + (meshPowerUp.Vertices.size() * mElementCount.powerUps)
		+ (meshPacMan.Vertices.size() * 1) + (meshGhost.Vertices.size() * 4);
	UINT countIndex = (meshBox.Indices.size() * mElementCount.walls) + (meshPellet.Indices.size() * mElementCount.pellets) + (meshPowerUp.Indices.size() * mElementCount.powerUps) 
		+ (meshPacMan.Indices.size() * 1) + (meshGhost.Indices.size() * 4);

	vertices.resize(countVertex);
	indices.resize(countIndex);
	/*for (int i = 0; i < mElementCount.walls; ++i)
	{
		for (size_t j = 0; j < meshBox.Vertices.size(); ++j)
		{
			vertices[i].pos = meshBox.Vertices[j].Position;
			vertices[i].normal = meshBox.Vertices[j].Normal;
			vertices[i].tex = meshBox.Vertices[j].TexC;
		}
	}*/

	float posX = lineWidth * -0.5f;
	float posY = lineCount * -0.5f;
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

	mElementOffsetsCounts.walls.vertexOffset = 0;
	mElementOffsetsCounts.walls.indexOffset = 0;
	mElementOffsetsCounts.walls.vertexCount = meshBox.Vertices.size() * mElementCount.walls;
	mElementOffsetsCounts.walls.indexCount = meshBox.Indices.size() * mElementCount.walls;
	mElementOffsetsCounts.walls.vertexSize = meshBox.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshBox.Indices.size();

	mElementOffsetsCounts.pellets.vertexOffset = startVertexPellet;
	mElementOffsetsCounts.pellets.indexOffset = startIndexPellet;
	mElementOffsetsCounts.pellets.vertexCount = meshPellet.Vertices.size() * mElementCount.pellets;
	mElementOffsetsCounts.pellets.indexCount = meshPellet.Indices.size() * mElementCount.pellets;
	mElementOffsetsCounts.walls.vertexSize = meshPellet.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshPellet.Indices.size();

	mElementOffsetsCounts.powerUps.vertexOffset = startVertexPowerUp;
	mElementOffsetsCounts.powerUps.indexOffset = startIndexPowerUp;
	mElementOffsetsCounts.powerUps.vertexCount = meshPowerUp.Vertices.size() * mElementCount.powerUps;
	mElementOffsetsCounts.powerUps.indexCount = meshPowerUp.Indices.size() * mElementCount.powerUps;
	mElementOffsetsCounts.walls.vertexSize = meshPowerUp.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshPowerUp.Indices.size();

	mElementOffsetsCounts.pacMan.vertexOffset = startVertexPacMan;
	mElementOffsetsCounts.pacMan.indexOffset = startIndexPacMan;
	mElementOffsetsCounts.pacMan.vertexCount = meshPacMan.Vertices.size() * 1;
	mElementOffsetsCounts.pacMan.indexCount = meshPacMan.Indices.size() * 1;
	mElementOffsetsCounts.walls.vertexSize = meshPacMan.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshPacMan.Indices.size();

	mElementOffsetsCounts.ghosts.vertexOffset = startVertexGhost;
	mElementOffsetsCounts.ghosts.indexOffset = startIndexGhost;
	mElementOffsetsCounts.ghosts.vertexCount = meshGhost.Vertices.size() * 4;
	mElementOffsetsCounts.ghosts.indexCount = meshGhost.Indices.size() * 4;
	mElementOffsetsCounts.walls.vertexSize = meshGhost.Vertices.size();
	mElementOffsetsCounts.walls.indexSize = meshGhost.Indices.size();

	UINT xCount = 0;
	UINT len = 0;
	for (int i = 0; i < mazeText.size(); ++i)
	{
		xCount = 0;
		for (int j = 0; j < mazeText[i].length(); ++j)
		{
			len = mazeText[i].length();
			if (mazeText[i][j] == L'X')	// wall
			{
				++xCount;
				for (size_t k = 0; k < meshBox.Vertices.size(); ++k, ++vCountWall)
				{
					vertices[vCountWall].pos.x = meshBox.Vertices[k].Position.x + posX;
					vertices[vCountWall].pos.y = meshBox.Vertices[k].Position.y;
					vertices[vCountWall].pos.z = meshBox.Vertices[k].Position.z + -posY;
					vertices[vCountWall].normal = meshBox.Vertices[k].Normal;
					vertices[vCountWall].tex = meshBox.Vertices[k].TexC;
				}
				for (size_t k = 0; k < meshBox.Indices.size(); ++k, ++iCountWall)
				{
					indices[iCountWall] = meshBox.Indices[k] + iBlockWall;
				}
				mBoxData.push_back(AABox(XMVectorSet(posX, 0.0f, -posY, 0.0f), 0.5f, 0.5f));
				iBlockWall += meshBox.Vertices.size();
				mMazeElements.push_back(ME_WALL);
			}
			if (mazeText[i][j] == L' ')	// pellets
			{
				for (size_t k = 0; k < meshPellet.Vertices.size(); ++k, ++vCountPellet)
				{
					vertices[vCountPellet].pos.x = meshPellet.Vertices[k].Position.x + posX;
					vertices[vCountPellet].pos.y = meshPellet.Vertices[k].Position.y;
					vertices[vCountPellet].pos.z = meshPellet.Vertices[k].Position.z + -posY;
					vertices[vCountPellet].normal = meshPellet.Vertices[k].Normal;
					vertices[vCountPellet].tex = meshPellet.Vertices[k].TexC;
				}
				for (size_t k = 0; k < meshPellet.Indices.size(); ++k, ++iCountPellet)
				{
					indices[iCountPellet] = meshPellet.Indices[k] + iBlockPellet;
				}
				iBlockPellet += meshPellet.Vertices.size();
				mMazeElements.push_back(ME_PELLET);
			}
			if (mazeText[i][j] == L'O')	// power up
			{
				for (size_t k = 0; k < meshPowerUp.Vertices.size(); ++k, ++vCountPowerUp)
				{
					vertices[vCountPowerUp].pos.x = meshPowerUp.Vertices[k].Position.x + posX;
					vertices[vCountPowerUp].pos.y = meshPowerUp.Vertices[k].Position.y;
					vertices[vCountPowerUp].pos.z = meshPowerUp.Vertices[k].Position.z + -posY;
					vertices[vCountPowerUp].normal = meshPowerUp.Vertices[k].Normal;
					vertices[vCountPowerUp].tex = meshPowerUp.Vertices[k].TexC;
				}
				for (size_t k = 0; k < meshPowerUp.Indices.size(); ++k, ++iCountPowerUp)
				{
					indices[iCountPowerUp] = meshPowerUp.Indices[k] + iBlockPowerUp;
				}
				iBlockPowerUp += meshPowerUp.Vertices.size();
				mMazeElements.push_back(ME_POWERUP);
			}
			if (mazeText[i][j] == L'M')	// Puck Man
			{
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
				mInitialPositions.pacMan = XMFLOAT3(posX + 0.5f, 0.0f, posY);
				iBlockPacMan += meshPowerUp.Vertices.size();
				mMazeElements.push_back(ME_NOTHING);
			}
			if (mazeText[i][j] == L'B' || mazeText[i][j] == L'I' || mazeText[i][j] == L'P' || mazeText[i][j] == L'C')	// Ghosts
			{
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
				switch (mazeText[i][j])
				{
				case L'B':
					mInitialPositions.blinky = XMFLOAT3(posX + 0.5f, 0.0f, -posY);
					break;
				case L'I':
					mInitialPositions.inky = XMFLOAT3(posX + 0.5f, 0.0f, -posY);
					break;
				case L'P':
					mInitialPositions.pinky = XMFLOAT3(posX + 0.5f, 0.0f, -posY);
					break;
				case L'C':
					mInitialPositions.clyde = XMFLOAT3(posX + 0.5f, 0.0f, -posY);
					break;
				}
				iBlockGhost += meshPowerUp.Vertices.size();
				mMazeElements.push_back(ME_NOTHING);
			}
			if (mazeText[i][j] == L'M')	// Really nothing
			{
				mMazeElements.push_back(ME_NOTHING);
			}
			++posX;
			//iBlock += iCount;
		}
		posX = lineWidth * -0.5f;
		++posY;
	}

	return true;
}

bool MazeLoader::IsBlocked(UINT row, UINT col)
{
	UINT index = (row * mMazeWidth) + col;

	return (mMazeElements[index] == ME_WALL);
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

