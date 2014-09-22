#pragma once
#include "d3dUtil.h"
#include "PathNode.h"
#include <map>

class Pathfinding
{
protected:
	UINT mTotalCols;
	UINT mTotalRows;

	std::map<PathNode*, bool> mOpenMap; //true = node is open, false = node is closed

public:
	Pathfinding();
	~Pathfinding();

	//std::vector<PathNode*> mOpenList;
	//std::vector<PathNode*> mClosedList;
	std::vector<PathNode*> FindPath(PathNode* start, PathNode* goal);
	void AddChild(int row, int col, PathNode* currNode, PathNode* goal);
	void InitializePathFinding();
	float GetDistance(PathNode* currNode, PathNode* parent);
	bool InClosedList(PathNode* n);
	bool InOpenList(PathNode* n);
};

