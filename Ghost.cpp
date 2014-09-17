#include "Ghost.h"

Ghost::Ghost()
{
}

Ghost::Ghost(UINT row, UINT col, FXMVECTOR vel, float radius)
{
	mRow = row;
	mCol = col;
	XMStoreFloat3(&mVel, vel);
	mRadius = radius;
}

Ghost::~Ghost()
{
}

void Ghost::Update()
{
	
}

std::vector<PathNode*> Ghost::FindPath(PathNode* start, PathNode* goal)
{
	std::vector<PathNode*> tempPath;

	mClosedList.clear();
	mOpenList.clear();

	PathNode* childNode;
	PathNode* currentNode = new PathNode(*start);
	//currentNode->combineNode(*currentNode, start);

	mOpenList.push_back(start);
	start->isOpen = true;

	while (*currentNode != *goal)
	{
		for (int i = 0; i < mOpenList.size(); ++i)
		{
			if (mOpenList[i]->fCost <= currentNode->fCost)
			{
				currentNode->combineNode(*currentNode, mOpenList[i]);
			}
		}
		//Did we reach the end? If so, stop
		if (*currentNode == *goal)
		{
			break;
		}
		//If not, we continue and remove the currentNode from the openList
			//Because the currentNode isn't in the openList, we will simply set isOpen to false
			//and isClosed to true
		currentNode->isOpen = false;
		mClosedList.push_back(currentNode);
		currentNode->isClosed = true;

		//Get adjacent walkable tiles
		for (int col = -1; col < 2; ++col)
		{
			for (int row = -1; row < 2; ++row)
			{
				//Skip if this lands on currentNode
				if (col == 0 && row == 0)
				{
					continue;
				}
				//If not, get the node
				
				//childNode = new PathNode(getNode(currentNode->getCol() + col, currentNode->getRow() + row));
				if (childNode->isClosed || !childNode->isWalkable)
				{
					continue;
				}

				//If we hit a corner
				/*if (col != 0 && row != 0)
				{
					//Check surroundings for walkable tiles and if in closed list
					if (!isWalkable(currentNode->getCol(), currentNode->getRow() + row) || getNode(currentNode->getCol(), currentNode->getRow() + row)->closed)
					{
						continue;
					}
					if (!isWalkable(currentNode->getCol() + col, currentNode->getRow()) || getNode(currentNode->getCol() + col, currentNode->getRow())->closed)
					{
						continue;
					}
				}
				*/
				//If it's already in the open list
				if (childNode->isOpen)
				{
					if (childNode->gCost > childNode->gCost + currentNode->gCost)
					{
						childNode->setParent(currentNode);
						childNode->calculateCosts(goal);
					}
				}
				else
				{
					//Add it to the openList with the current node as the parent
					mOpenList.push_back(childNode);
					childNode->isOpen = true;
					childNode->setParent(currentNode);
					childNode->calculateCosts(goal);
				}
			}
		}
	}
	//Reset everything
	for (int i = 0; i < mOpenList.size(); ++i)
	{
		mOpenList[i]->isOpen = false;
	}
	for (int i = 0; i < mClosedList.size(); ++i)
	{
		mClosedList[i]->isClosed = false;
	}

	//Populate the path vector
	while (currentNode->mParent != NULL && currentNode != start)
	{
		tempPath.push_back(currentNode);
		currentNode = currentNode->getParent();
	}
	return tempPath;
}

PathNode getNode(int row, int col)
{
	PathNode tempNode(row, col, 0, 0, NULL, "forward");
	return tempNode;
}