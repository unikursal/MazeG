#include "Maze.h"

#include<iostream>
#include<fstream>

Maze::Maze()
{
	srand(time(0));
	int i, j;

	map = new int*[HEIGHT];

	for (i = 0; i < HEIGHT; i++)
		map[i] = new int[WIDTH];
}


Maze::~Maze()
{
	int i;
	for (i = 0; i < HEIGHT; i++)
		delete [] map[i];

	delete [] map;
}

bool Maze::generateMaze()
{
	int i, j, numberSet = 1;
	bool firstRow = true;

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			map[i][j] = 0;

	map[0][0] = numberSet;
	numberSet++;
	for (j = 1; j < WIDTH; j++)
		map[0][j] = BORDER;

	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			map[i][j] = numberSet;
			numberSet++;

		}

		//set wall
		for (j = 0; j < WIDTH - 1; j++)
		{
			if (setBorder() && map[i - 1][j] == BORDER)
				map[i][j] = BORDER;
			else
				map[i][j + 1] = map[i][j];
		}

		/*
			set floor
			map[i][j] == -1 - is floor
		*/
		if (i < HEIGHT - 2)
		{
			i++;
			int roadForSet = -1;

			for (j = 0; j < WIDTH; j++)
			{
				if (map[i - 1][j] == BORDER)
				{
					map[i][j] = BORDER;
					continue;
				}

				map[i][j] = map[i - 1][j];

				if (map[i][j] == roadForSet)
				{
					map[i][j] = BORDER;
				}
				else
				{
					if (j < WIDTH - 1 && map[i][j] == map[i - 1][j + 1] && setBorder())
					{
						map[i][j] = BORDER;
					}
					else
						roadForSet = map[i][j];
				}

			}
		}
	}

	char str[WIDTH + 2];
	std::fstream fs;
	fs.open("D:\\a.txt", std::fstream::app);
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (map[i][j] == BORDER)
				str[j] = 'X';
			else
				str[j] = ' ';
		}

		str[WIDTH] = '\n';
		str[WIDTH + 1] = '\0';

		fs << str;
	}

	fs.close();

	return 0;
}

void Maze::updateSceneFrame(SceneFrame& sf, int xC, int yC)
{
	int	i, j, startX = sf.cell.startFrameX, startY = sf.cell.startFrameY, 
		endX = sf.cell.startFrameX + sf.cell.widthFrame, endY = sf.cell.startFrameY + sf.cell.heightFrame;

	if (xC < sf.pixel.startFrameX + sf.pixel.wBias && xC > sf.pixel.wBias)
	{
		startX = (sf.cell.startFrameX > sf.cell.wBias) ? sf.cell.startFrameX - sf.cell.wBias : 0;
		endX = sf.cell.startFrameX;

		startY = sf.cell.startFrameY;
		endY = sf.cell.startFrameY + sf.cell.heightFrame;

		sf.pixel.startFrameX -= (endX - startX) * sf.wCell;
		sf.cell.startFrameX -= (endX - startX);
	}

	if (xC > sf.pixel.startFrameX + sf.pixel.wBuffer)
	{
		startX = sf.cell.startFrameX + sf.cell.widthFrame;
		endX = (startX + sf.cell.wBias) > WIDTH ? WIDTH : startX + sf.cell.wBias;

		startY = sf.cell.startFrameY;
		endY = sf.cell.startFrameY + sf.cell.heightFrame;

		sf.pixel.startFrameX += (endX - startX) * sf.wCell;
		sf.cell.startFrameX += endX - startX;
	}

	if (yC < sf.pixel.startFrameY + sf.pixel.hBias && yC > sf.pixel.hBias)
	{
		startY = (sf.cell.startFrameY > sf.cell.hBias) ? sf.cell.startFrameY - sf.cell.hBias : 0;
		endY = sf.cell.startFrameY;

		startX = sf.cell.startFrameX;
		endX = sf.cell.startFrameX + sf.cell.widthFrame;

		sf.pixel.startFrameY -= (endY - startY) * sf.hCell;
		sf.cell.startFrameY -= (endY - startY);
	}

	if (yC > sf.pixel.startFrameY + sf.pixel.hBuffer)
	{
		startY = sf.cell.startFrameY + sf.cell.heightFrame;
		endY = (startY + sf.cell.hBias) > HEIGHT ? HEIGHT : startY + sf.cell.hBias;

		startX = sf.cell.startFrameX;
		endX = sf.cell.startFrameX + sf.cell.widthFrame;

		sf.pixel.startFrameY += (endY - startY) * sf.hCell;
		sf.cell.startFrameY += (endY - startY);
	}

	for (i = startY; i < endY; i++)
		for (j = startX; j < endX; j++)
			if (map[i][j] == BORDER)
				sf.coordinates.push_back({j * sf.wCell, i * sf.hCell});
}