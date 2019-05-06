#include "SceneFrame.h"

SceneFrame::SceneFrame()
{
}

SceneFrame::~SceneFrame()
{
}

std::vector<std::pair<int, int>> SceneFrame::getCoordinates()
{
	return coordinates;
}

void SceneFrame::deleteCoordElements()
{
	coordinates.clear();
}

void SceneFrame::initialize(int w, int h, int ww, int hw)
{
	width = w;
	height = h;
	wCell = ww;
	hCell = hw;

	hCountCells = (double)height / hw;
	wCountCells = (double)width / ww;

	

	pixel.wBias = width * 0.5;
	pixel.hBias = height * 0.5;

	pixel.wBuffer = width;
	pixel.hBuffer = height;

	pixel.startFrameX = 0;
	pixel.startFrameY = 0;
	pixel.widthFrame = 2 * width;
	pixel.heightFrame = 2 * height;

	cell.wBias = pixel.wBias / wCell;
	cell.hBias = pixel.hBias / hCell;

	cell.startFrameX = 0;
	cell.startFrameY = 0;

	cell.widthFrame = pixel.widthFrame / wCell;
	cell.heightFrame = pixel.heightFrame / hCell;
}
