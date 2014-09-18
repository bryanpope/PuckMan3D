#pragma once
#include "d3dUtil.h"
#include "PathNode.h"

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	std::vector<PathNode*> mOpenList;
	std::vector<PathNode*> mClosedList;

	std::vector<PathNode*> FindPath(PathNode* start, PathNode* goal);
	//void AddChild(PathNode* childNode, PathNode* currNode, PathNode* goal);
	void InitializePathFinding();
	float GetDistance(PathNode* currNode, PathNode* parent);
	bool InClosedList(PathNode* n);
	bool InOpenList(PathNode* n);
	PathNode getNode(int row, int col);
};

