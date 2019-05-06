#ifndef __MAZE_H__
#define __MAZE_H__

#include<ctime>
#include<cstdlib>
#include<cmath>

#include "SceneFrame.h"

class Maze
{
public:
	static const int WIDTH = 30, HEIGHT = 40;

	Maze();
	~Maze();

	bool generateMaze();
	void updateSceneFrame(SceneFrame& sf, int x, int y);

private:
	const int BORDER = - 1;
	int **map;

	bool setBorder();
};

inline bool Maze::setBorder()
{
	int max = 10, chance = 4, i;
	i = std::rand() % max;

	if (i < chance)
		return true;

	return false;
}
#endif

