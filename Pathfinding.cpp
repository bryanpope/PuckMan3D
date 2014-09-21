#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

/*std::vector<PathNode*> Pathfinding::FindPath(PathNode* start, PathNode* goal)
{
	std::vector<PathNode*> tempPath;

	//mClosedList.clear();
	//mOpenList.clear();
	mOpenMap.clear();

	PathNode* childNode;
	PathNode* currentNode = new PathNode(*start);
	currentNode->combineNode(*currentNode, start);

	//mOpenList.push_back(start);
	mOpenMap.emplace(start, true); //Put the start node into the openList, marking it as true here means it is open
	//start->isOpen = true;

	while (currentNode != goal)
	{
		for (auto it : mOpenMap)
		{
			if (it.first->mFCost <= currentNode->mFCost)
			{
				currentNode->combineNode(*currentNode, it.first);
			}
		}
		//Did we reach the end? If so, stop
		if (currentNode == goal)
		{
			break;
		}
		//If not, we continue and remove the currentNode from the openList
			//Because the currentNode isn't in the openList, we will simply set isOpen to false
			//and isClosed to true

		auto temp = mOpenMap.find(currentNode); //Find the currentNode in the map
		temp->second = false; //Set the currentNode to false, as this will denote that it is in the closed list

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
				if (col != 0 && row != 0)
				{
					//Check surroundings for walkable tiles and if in closed list
					/*if (!isWalkable(currentNode->getCol(), currentNode->getRow() + row) || getNode(currentNode->getCol(), currentNode->getRow() + row)->closed)
					{
						continue;
					}
					if (!isWalkable(currentNode->getCol() + col, currentNode->getRow()) || getNode(currentNode->getCol() + col, currentNode->getRow())->closed)
					{
						continue;
					}*
				}
				
				//If it's already in the open list
				if (childNode->isOpen)
				{
					if (childNode->mGCost > childNode->mGCost + currentNode->mGCost)
					{
						childNode->setParent(currentNode);
						childNode->calculateCosts(goal);
					}
				}
				else
				{
					//Add it to the openList with the current node as the parent
					//mOpenList.push_back(childNode);
					mOpenMap.emplace(childNode);
					auto superTemp = mOpenMap.find(childNode);
					superTemp->second = true;
					//childNode->isOpen = true;
					//superTemp->first->setParent(currentNode);
					childNode->setParent(currentNode);
					//superTemp->first->calculateCosts(goal);
					childNode->calculateCosts(goal);
				}
			}
		}
	}
	//Reset everything
	for (auto it : mOpenMap)
	{
		it.second = false;
	}

	//Populate the path vector
	while (currentNode->mParent != NULL && currentNode != start)
	{
		tempPath.push_back(currentNode);
		currentNode = currentNode->getParent();
	}
	return tempPath;
}*/

PathNode getNode(int row, int col)
{
	PathNode tempNode(row, col, 0, 0, NULL, "forward");
	return tempNode;
}