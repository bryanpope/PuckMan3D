#pragma once
#include "d3dUtil.h"
#include "PathNode.h"

class Ghost
{
protected:
	enum GHOST_STATES
	{
		SCATTER = 1,
		CHASE = 2,
		FRIGHTENED = 3,
		DEAD = 4
		//IN_TUNNEL = 5 //Maybe make this a state?
	};

	XMFLOAT3 mPos; //Originally set as XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f)
	XMFLOAT3 mVel; //Originally set as XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
	float mRadius; //Originally set as 0.75f
	int mLevelNumber;

	GHOST_STATES mGhostStates;

protected:
	//Pathfinding stuff -- make different class or keep as is?
	std::vector<PathNode*> mOpenList;
	std::vector<PathNode*> mClosedList;

	std::vector<PathNode*> FindPath(PathNode* start, PathNode* goal);
	void AddChild(PathNode* childNode, PathNode* currNode, PathNode* goal);
	float GetDistance(PathNode* currNode, PathNode* parent);
	bool InClosedList(PathNode* n);
	bool InOpenList(PathNode* n);

public:
	Ghost();
	Ghost(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Ghost();
	virtual void Update();
};