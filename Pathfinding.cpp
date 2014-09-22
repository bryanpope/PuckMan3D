#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

std::vector<PathNode*> Pathfinding::FindPath(PathNode* start, PathNode* goal)
{
	std::vector<PathNode*> tempPath;

	mOpenMap.clear();

	PathNode* childNode;
	PathNode* currentNode = new PathNode(*start);
	currentNode->combineNode(currentNode, start);

	mOpenMap.insert(std::make_pair(start, true)); //Put the start node into the openList, marking it as true here means it is open

	while (currentNode != goal)
	{
		for (auto it : mOpenMap)
		{
			//Compare currentNode's fCost to the current place in the map
			if (it.first->mFCost <= currentNode->mFCost)
			{
				currentNode->combineNode(currentNode, it.first);
			}
		}
		//Did we reach the end? If so, stop
		if (currentNode == goal)
		{
			break;
		}
		//If not, we continue and remove the currentNode from the openList
		auto temp = mOpenMap.find(currentNode); //Find the currentNode in the map
		temp->second = false; //Set the currentNode to false, as this will denote that it is in the closed list

		//Get adjacent walkable tiles
		//Move the child node one node to the right to get the node to the right of currentNode
		AddChild(currentNode->mRow, currentNode->mCol + 1, currentNode, goal);

		//Move the child node to the left to get the node to the left of currentNode
		AddChild(currentNode->mRow, currentNode->mCol - 1, currentNode, goal);

		//Move the child node up one row to get the node above currentNode
		AddChild(currentNode->mRow + 1, currentNode->mCol, currentNode, goal);

		//Finally, move the child node to the bottom, to get the node one below currentNode
		AddChild(currentNode->mRow - 1, currentNode->mCol, currentNode, goal);
		/*for (int col = -1; col < 2; ++col)
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
					}
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
					mOpenMap.insert(std::make_pair(childNode, true));
					childNode->setParent(currentNode);
					childNode->calculateCosts(goal);
				}
			}
		}*/
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
}

void Pathfinding::AddChild(int row, int col, PathNode* currNode, PathNode* goal)
{

}

PathNode Pathfinding::getNode(int row, int col)
{
	PathNode tempNode(row, col, 0, 0, NULL, "forward");
	return tempNode;
}