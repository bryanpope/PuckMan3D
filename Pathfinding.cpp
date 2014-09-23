#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

std::list<PathNode*> Pathfinding::FindPath(PathNode* start, PathNode* goal)
{
	//mOpenMap.clear();
	mOpenList.clear();
	mClosedSet.clear();

	std::list<PathNode*> tempPath;

	PathNode* currentNode = new PathNode(*start);
	currentNode->combineNode(currentNode, start);
	

	//mOpenMap.insert(std::make_pair(start, true)); //Put the start node into the openList, marking it as true here means it is open
	mOpenList.push_back(start);
	start->isOpen = true;

	if (currentNode == goal)
	{
		std::cout << "We're already there" << std::endl;
		return tempPath;
	}

	while ((currentNode->xPos != goal->xPos) && (currentNode->zPos != goal->zPos))
	{		
		std::cout << "CurrNode X: " << currentNode->xPos << ", CurrNode Z: " << currentNode->zPos << std::endl;
		PathNode tempChildNode(*currentNode);

		//Get adjacent walkable tiles
		//Move the child node one node to the right to get the node to the right of currentNode
		tempChildNode.xPos++;
		AddChild(tempChildNode, currentNode, goal);

		//Move the child node to the left to get the node to the left of currentNode
		tempChildNode.xPos -= 2;
		AddChild(tempChildNode, currentNode, goal);

		//Move the child node up one row to get the node above currentNode
		tempChildNode.xPos++;
		tempChildNode.zPos++;
		AddChild(tempChildNode, currentNode, goal);

		//Finally, move the child node to the bottom, to get the node one below currentNode
		tempChildNode.zPos -= 2;
		AddChild(tempChildNode, currentNode, goal);

		mClosedSet.insert(currentNode);

		if (mOpenList.size() > 0)
		{
			currentNode = mOpenList.back();
			mOpenList.remove(currentNode);
		}
		//std::cout << "openList.size " << mOpenList.size() << std::endl;
		//std::cout << "closedSet.size " << mClosedSet.size() << std::endl;
	}
	//Populate and create the path vector
	while (currentNode->parent != NULL && currentNode != start)
	{
		tempPath.push_back(currentNode);
		currentNode = currentNode->getParent();
	}
	tempPath.reverse();
	return tempPath;
}

void Pathfinding::AddChild(PathNode childNode, PathNode* currNode, PathNode* goal)
{
	//Check surroundings for walkable tiles and if in closed list
	if ((childNode.xPos >= -14 && childNode.zPos >= -15.5) && (childNode.xPos<= 14 && childNode.zPos <= 15.5))
	{
		if (!MazeLoader::IsBlocked(childNode.zPos, childNode.xPos))
		{
			if (!InClosedList(&childNode))
			{
				childNode.facing = currNode->facing;
				int g = currNode->gCost + childNode.getDistanceFromParent(childNode, currNode);
				int f = g + childNode.getDistance(goal, childNode.xPos, childNode.zPos);

				//If it's already in the open list
				if (InOpenList(&childNode))
				{
					if (childNode.gCost > g)
					{
						childNode.setParent(currNode);
						childNode.fCost = f;
						childNode.gCost = g;
					}
				}
				else
				{
					//Add it to the openList with the current node as the parent
					PathNode* newChildNode = new PathNode(childNode.xPos, childNode.zPos, g, f, currNode, "");
					mOpenList.push_back(newChildNode);
				}
			}
		}
	}
}

bool Pathfinding::InOpenList(PathNode* n)
{
	auto result = std::find_if(mOpenList.begin(), mOpenList.end(), compare(n->xPos, n->zPos));
	if (result == mOpenList.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Pathfinding::InClosedList(PathNode* n)
{
	auto result = std::find_if(mClosedSet.begin(), mClosedSet.end(), compare(n->xPos, n->zPos));
	if (result == mClosedSet.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}