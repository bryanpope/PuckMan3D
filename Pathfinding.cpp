#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

std::vector<PathNode*> Pathfinding::FindPath(PathNode* start, PathNode* goal)
{
	mOpenList.clear();
	mClosedSet.clear();

	std::vector<PathNode*> retPath;

	PathNode* currentNode = new PathNode(*start);
	currentNode->combineNode(currentNode, start);
	int goalRow = (MazeLoader::GetMazeHeight()) - (int)round(goal->zPos + 15.5f);
	int goalCol = ((int)round(goal->xPos + 14.5f) - 1);
	if ((goalCol >= 0 && goalRow >= 0) && ((UINT)goalCol <= MazeLoader::GetMazeWidth() && (UINT)goalRow <= MazeLoader::GetMazeHeight()))
	{
		if (!MazeLoader::IsBlocked(goalRow, goalCol))
		{
			while (!ArrivedAtEnd(currentNode, goal))
			{
				PathNode tempChildNode(*currentNode);

				//Get adjacent walkable tiles
				//Move the child node one node to the right to get the node to the right of currentNode
				tempChildNode.xPos++;
				AddChild(tempChildNode, currentNode, goal, Direction::DIRECTION::EAST);

				//Move the child node to the left to get the node to the left of currentNode
				tempChildNode.xPos -= 2;
				AddChild(tempChildNode, currentNode, goal, Direction::DIRECTION::WEST);

				//Move the child node up one row to get the node above currentNode
				tempChildNode.xPos++;
				tempChildNode.zPos++;
				AddChild(tempChildNode, currentNode, goal, Direction::DIRECTION::NORTH);

				//Finally, move the child node to the bottom, to get the node one below currentNode
				tempChildNode.zPos -= 2;
				AddChild(tempChildNode, currentNode, goal, Direction::DIRECTION::SOUTH);

				mClosedSet.insert(currentNode);

				mOpenList.sort(PathNode::FCostSort());

				if (mOpenList.size() > 0)
				{
					currentNode = mOpenList.back();
					mOpenList.remove(currentNode);
				}
				else
				{
					break;
				}
			}
			//Populate and create the path vector
			while (currentNode->parent != NULL && currentNode != start)
			{
				retPath.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			//retPath.reverse();
			std::reverse(retPath.begin(), retPath.end());
			mOpenList.clear();
			mClosedSet.clear();
			return retPath;
		}
		else
		{
			return retPath;
		}
	}
	else
	{
		return retPath;
	}
}

bool Pathfinding::ArrivedAtEnd(PathNode* currNode, PathNode* goal)
{
	return (currNode->xPos == goal->xPos) && (currNode->zPos == goal->zPos);
}

void Pathfinding::AddChild(PathNode childNode, PathNode* currNode, PathNode* goal, Direction::DIRECTION facing)
{
	int col = (int)round(childNode.xPos + 14.5f) - 1;
	int row = (MazeLoader::GetMazeHeight()) - (int)round(childNode.zPos + 15.5f);

	//Check surroundings for walkable tiles and if in closed list
	if ((col >= 0 && row >= 0) && ((UINT)col <= MazeLoader::GetMazeWidth() && (UINT)row <= MazeLoader::GetMazeHeight()))
	{
		if (!MazeLoader::IsBlocked(row, col))
		{
			if (!InClosedList(&childNode))
			{
				childNode.facing = facing;
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
					PathNode* newChildNode = new PathNode(childNode.xPos, childNode.zPos, g, f, currNode, facing);
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