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
		OffsetsCounts floors;
		OffsetsCounts wallsBent;
		OffsetsCounts wallsStraight;
		OffsetsCounts pellets;
		OffsetsCounts powerUps;
		OffsetsCounts pacMan;
		OffsetsCounts ghosts;
		OffsetsCounts triggers;
		OffsetsCounts traps;
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
	struct MazeElementSpecs
	{
		XMFLOAT4X4 world;
		XMFLOAT4 colour;
		XMFLOAT3 pos;
		XMFLOAT3 vel;
		bool isShown;			// true - element is displayed
		bool isCollider;		// true - element triggers a collision

		MazeElementSpecs(XMFLOAT4X4 w, XMFLOAT4 c, bool s, bool t)
		{
			world = w;
			colour = c;
			pos.x = world._41;
			pos.y = world._42;
			pos.z = world._43;
			XMStoreFloat3(&vel, XMVectorZero());
			isShown = s;
			isCollider = t;
		}
	};
	enum MazeElements
	{
		ME_WALL,
		ME_PELLET,
		ME_PELLET_DIVERGENT,
		ME_PELLET_TRIGGER,
		ME_PELLET_TRAP,
		ME_POWERUP,
		ME_BLANK,
		ME_NOTHING,
		ME_NOTHING_DIVERGENT

	};

	static bool Load(ID3D11Device* device, std::string filename, std::vector<Vertex::NormalTexVertex>& vertices, std::vector<UINT>& indices,
		std::vector<Vertex::InstancedData>& instFloor,
		std::vector<Vertex::InstancedData>& instWallsBent, std::vector<Vertex::InstancedData>& instWallsStraight,
		std::vector<Vertex::InstancedData>& instPellets, std::vector<Vertex::InstancedData>& instPowerUps,
		std::vector<Vertex::InstancedData>& instPacMans, std::vector<Vertex::InstancedData>& instGhosts,
		std::vector<Vertex::InstancedData>& instTriggers, std::vector<Vertex::InstancedData>& instTraps);

	static const OffsetsCountsMazeElements& GetOffsetsCounts(){ return mElementOffsetsCounts; }
	static const InitialPosition& GetInitialPos(){ return mInitialPositions; }
	static const std::vector<AABox> GetWallCollisionData(){ return mBoxData; }
	static const std::vector<AABox> GetTriggerCollisionData(){ return mTriggerData; }
	static const std::vector<AABox> GetTrapCollisionData(){ return mTrapData; }
	static bool IsBlocked(UINT row, UINT col);
	static bool IsDivergent(UINT row, UINT col);
	static std::vector<MazeElementSpecs> GetFloorData(){ return mFloor; }
	static std::vector<MazeElementSpecs> GetWallBentData(){ return mWallsBent; }
	static std::vector<MazeElementSpecs> GetWallStraightData(){ return mWallsStraight; }
	static std::vector<MazeElementSpecs> GetPelletData(){ return mPellets; }
	static std::vector<MazeElementSpecs> GetPowerUpData(){ return mPowerUps; }
	static std::vector<MazeElementSpecs> GetPacManData(){ return mPacMans; }
	static std::vector<MazeElementSpecs> GetGhostData(){ return mGhosts; }
	static std::vector<MazeElementSpecs> GetTriggerData(){ return mTriggers; }
	static std::vector<MazeElementSpecs> GetTrapData(){ return mTraps; }
	//static void EraseWall(UINT index){ mWalls.erase(mWalls.begin() + index); }
	static void ErasePellet(UINT index){ mPellets.erase(mPellets.begin() + index); }
	static void ErasePowerUp(UINT index){ mPowerUps.erase(mPowerUps.begin() + index); }
	static void ErasePacMan(UINT index){ mPacMans.erase(mPacMans.begin() + index); }
	static void SetPacManPos(FXMVECTOR pos, UINT index);
	static void SetPacManVel(FXMVECTOR vel, UINT index);
	static void SetGhostPos(FXMVECTOR pos, UINT index);
	static void SetGhostVel(FXMVECTOR vel, UINT index);
	static void SetGhostColour(XMFLOAT4 col, UINT index);
	static void HideGhost(UINT index){ mGhosts[index].isShown = mGhosts[index].isCollider = false; }
	static void ShowGhost(UINT index){ mGhosts[index].isShown = mGhosts[index].isCollider = true; }

	static void RemovePellet(UINT index);
	static void RemovePowerUp(UINT index);
	static void RemoveLastPacMan();
	static void ResetPellets();
	static void ResetPowerUps();
	static void ResetPacMan();
	static void ResetPacManPosition();
	static void SiezureInducingPowerUp();

	static UINT GetMazeWidth(){ return mMazeWidth; }
	static UINT GetMazeHeight(){ return mMazeHeight; }
	static UINT GetEatableCount(){ return mElementCount.pellets + mElementCount.powerUps; }

	static bool IsPellet(UINT row, UINT col);

	static const float RADIUS_PAC_MAN;
	static const float RADIUS_GHOST;
	static const float RADIUS_PELLET;
	static const float RADIUS_POWERUP;

private:
	struct MazeElementsWallTypesCounts
	{
		UINT cornerTopLeft;
		UINT cornerTopRight;
		UINT cornerBottomRight;
		UINT cornerBottomLeft;
		UINT vertical;
		UINT horizontal;
	};
	struct MazeElementsCounts
	{
		UINT floors;
		MazeElementsWallTypesCounts walls;
		UINT pellets;
		UINT powerUps;
		UINT emptySpaces;
		UINT triggers;
		UINT traps;
	};
	static MazeElementsCounts mElementCount;

	static OffsetsCountsMazeElements mElementOffsetsCounts;
	static InitialPosition mInitialPositions;
	static std::vector<AABox> mBoxData;
	static std::vector<AABox> mTriggerData;
	static std::vector<AABox> mTrapData;
	static std::vector<MazeElements> mMazeElements;
	static std::vector<MazeElements> mMazeElementsModify;
	static std::vector<MazeElementSpecs> mFloor;
	static std::vector<MazeElementSpecs> mWallsBent;
	static std::vector<MazeElementSpecs> mWallsStraight;
	static std::vector<MazeElementSpecs> mPellets;
	static std::vector<MazeElementSpecs> mPowerUps;
	static std::vector<MazeElementSpecs> mPacMans;
	static std::vector<MazeElementSpecs> mGhosts;
	static std::vector<MazeElementSpecs> mTriggers;
	static std::vector<MazeElementSpecs> mTraps;
	static UINT mMazeWidth;
	static UINT mMazeHeight;

	static std::vector<Vertex::NormalTexVertex> mCylVerticesBent;
	static std::vector<UINT> mCylIndicesBent;
	static std::vector<MeshGeometry::Subset> mCylSubsetsBent;
	static std::vector<Vertex::NormalTexVertex> mCylVerticesStraight;
	static std::vector<UINT> mCylIndicesStraight;
	static std::vector<MeshGeometry::Subset> mCylSubsetsStraight;

	//static UINT AddVertex(Vertex::NormalTexVertex, std::vector<Vertex::NormalTexVertex>& vertBuf);

};

