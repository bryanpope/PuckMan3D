#pragma once

#include "d3dUtil.h"
#include <map>
#include "Vertex.h"
#include "MeshGeometry.h"
#include "BasicMeshGeometry.h"

class MazeLoader
{
public: 
	static const UINT MAX_VERTICES;
	static const UINT MAX_LINE_WIDTH;

public:
	MazeLoader(void);
	~MazeLoader(void);
	struct OffsetsCounts
	{
		UINT vertexCount;
		UINT vertexOffset;
		UINT vertexSize;
		UINT indexCount;
		UINT indexOffset;
		UINT indexSize;
	};
	struct OffsetsCountsMazeElements
	{
		OffsetsCounts walls;
		OffsetsCounts pellets;
		OffsetsCounts powerUps;
		OffsetsCounts pacMan;
		OffsetsCounts ghosts;
	};
	struct InitialPosition
	{
		XMFLOAT3 pacMan;
		XMFLOAT3 blinky;
		XMFLOAT3 inky;
		XMFLOAT3 pinky;
		XMFLOAT3 clyde;
	};
	struct AABox
	{
		XMFLOAT3 pos;
		float width; //x size
		float depth; //z size

		XMFLOAT3 min;
		XMFLOAT3 max;

		AABox(FXMVECTOR pos, float width, float depth)
		{
			XMStoreFloat3(&this->pos, pos);
			this->width = width;
			this->depth = depth;
			min.x = this->pos.x - (width / 2.0f);
			max.x = this->pos.x + (width / 2.0f);
			min.y = 0;
			max.y = 0.75f;
			min.z = this->pos.z - (depth / 2.0f);
			max.z = this->pos.z + (depth / 2.0f);
		}
	};
	enum MazeElements
	{
		ME_WALL,
		ME_PELLET,
		ME_POWERUP,
		ME_NOTHING
	};

	static bool Load(ID3D11Device* device, std::string filename, std::vector<Vertex::NormalTexVertex>& vertices, std::vector<UINT>& indices);

	static const OffsetsCountsMazeElements& GetOffsetsCounts(){ return mElementOffsetsCounts; }
	static const InitialPosition& GetInitialPos(){ return mInitialPositions; }
	static const std::vector<AABox> GetWallData(){ return mBoxData; }
	static bool IsBlocked(UINT row, UINT col);
	static bool IsPellet(UINT row, UINT col);

	static const float RADIUS_PAC_MAN;
	static const float RADIUS_GHOST;
	static const float RADIUS_PELLET;
	static const float RADIUS_POWERUP;

private:
	struct MazeElementsCounts
	{
		UINT walls;
		UINT pellets;
		UINT powerUps;
		UINT emptySpaces;
	};
	static MazeElementsCounts mElementCount;

	static OffsetsCountsMazeElements mElementOffsetsCounts;
	static InitialPosition mInitialPositions;
	static std::vector<AABox> mBoxData;
	static std::vector<MazeElements> mMazeElements;
	static UINT mMazeWidth;

	//static UINT AddVertex(Vertex::NormalTexVertex, std::vector<Vertex::NormalTexVertex>& vertBuf);

};

