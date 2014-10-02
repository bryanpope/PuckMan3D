#pragma once
#include "d3dUtil.h"
#include "PathNode.h"
#include "MazeLoader.h"
#include <set>
#include <list>
#include <iostream>

class Pathfinding
{
protected:
	struct compare
	{
		int x;
		int z;
		compare(int xPos, int zPos) : x(xPos), z(zPos) { }

		bool operator ()(PathNode* const& obj) const { return obj->xPos == x && obj->zPos == z; }
	};
	UINT mTotalCols;
	UINT mTotalRows;

	std::list<PathNode*> mOpenList;
	std::set<PathNode*> mClosedSet;

	bool InClosedList(PathNode* n);
	bool InOpenList(PathNode* n);

public:
	Pathfinding();
	~Pathfinding();

	std::vector<PathNode*> FindPath(PathNode* start, PathNode* goal);
	void AddChild(PathNode childNode, PathNode* currNode, PathNode* goal, std::string facing);
	void InitializePathFinding();
	float GetDistance(PathNode* currNode, PathNode* parent);
	bool ArrivedAtEnd(PathNode* currNode, PathNode* goal);
};