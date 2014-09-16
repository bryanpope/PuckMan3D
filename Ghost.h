#pragma once
#include "d3dUtil.h"
#include <cmath>

struct Node
{
	int row;
	int col;
	int gCost;
	int hCost;
	int fCost;
	bool isWalkable;
	bool isOpen;
	bool isClosed;
	std::string facing;
	Node* mParent;

	Node combineNode(Node initial, Node* target)
	{
		initial.col = target->col;
		initial.row = target->row;
		initial.gCost = target->gCost;
		initial.fCost = target->fCost;
		initial.facing = target->facing;

		return initial;
	}

	Node* getParent()
	{
		return mParent;
	}

	int getGCost(Node* n)
	{
		return n->gCost + ((col == n->col || row == n->row) ? 10 : 14);
	}

	int getHCost(Node* n)
	{
		return (abs(n->col - col) + abs(n->row - row)) * 10;
	}

	int getRow()
	{
		return row;
	}

	int getCol()
	{
		return col;
	}

	void setParent(Node* parent)
	{
		mParent = parent;
	}

	void calculateCosts(Node* goal)
	{
		gCost = getGCost(mParent);
		hCost = getHCost(mParent);
		fCost = gCost + hCost;
	}

	//May not need these overloads, currently work in progress
	bool operator==(const Node& rhs)
	{
		if (row == rhs.row && col == rhs.col && gCost == rhs.gCost && fCost == rhs.fCost && facing == rhs.facing)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!=(const Node& rhs)
	{
		if (row != rhs.row && col != rhs.col)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator=(const Node& rhs)
	{
		row = rhs.row;
		col = rhs.col;
		gCost = rhs.gCost;
		fCost = rhs.fCost;
		facing = rhs.facing;
		isWalkable = rhs.isWalkable;
		isOpen = rhs.isOpen;
		isClosed = rhs.isOpen;
	}
};

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

protected:
	//Pathfinding stuff -- make different class or keep as is?
	std::vector<Node*> mOpenList;
	std::vector<Node*> mClosedList;

	std::vector<Node*> FindPath(Node* start, Node* goal);
	void AddChild(Node* childNode, Node* currNode, Node* goal);
	float GetDistance(Node* currNode, Node* parent);
	bool InClosedList(Node* n);
	bool InOpenList(Node* n);

public:
	Ghost();
	Ghost(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Ghost();
	virtual void Update();
};