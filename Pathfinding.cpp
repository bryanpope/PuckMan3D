#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

std::list<PathNode*> Pathfinding::FindPath(PathNode* start, PathNode* goal)
{
	mOpenList.clear();
	mClosedSet.clear();

	std::list<PathNode*> tempPath;

	PathNode* currentNode = new PathNode(*start);
	currentNode->combineNode(currentNode, start);
	

	while (!ArrivedAtEnd(currentNode, goal))
	{		
		if (currentNode == goal)
		{
			break;
		}
		//std::cout << "CurrNode: " << currentNode->xPos << ", " << currentNode->zPos << " gCost: " << currentNode->gCost << " fCost: " << currentNode->fCost << std::endl;
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

		mOpenList.sort(PathNode::FCostSort());

		if (mOpenList.size() > 0)
		{
			currentNode = mOpenList.back();
			mOpenList.remove(currentNode);
		}
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

bool Pathfinding::ArrivedAtEnd(PathNode* currNode, PathNode* goal)
{
	return (currNode->xPos == goal->xPos) && (currNode->zPos == goal->zPos);
}

void Pathfinding::AddChild(PathNode childNode, PathNode* currNode, PathNode* goal)
{
	int currNodeCol = (MazeLoader::GetMazeWidth()) - (int)floor(currNode->xPos + 14.5f);
	int currNodeRow = (MazeLoader::GetMazeHeight() - 1) - (int)floor(currNode->zPos + 15.5f);

	int col = (MazeLoader::GetMazeWidth()) - (int)floor(childNode.xPos + 14.5f);
	int row = (MazeLoader::GetMazeHeight() - 1) - (int)floor(childNode.zPos + 15.5f);

	std::cout << "CurrNode Row " << currNodeRow << ", Col " << currNodeCol << std::endl;
	std::cout << "Row " << row << ", Col " << col << std::endl;
	//Check surroundings for walkable tiles and if in closed list
	if ((col >= 0 && row >= 0) && (col <= MazeLoader::GetMazeWidth() && row <= MazeLoader::GetMazeHeight()))
	{
		/*if (MazeLoader::IsBlocked(row, col))
		{
			std::cout << "Node at " << row << ", " << col << " is blocked" << std::endl;
		}
		if (!MazeLoader::IsBlocked(row, col))
		{*/
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
		//}
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