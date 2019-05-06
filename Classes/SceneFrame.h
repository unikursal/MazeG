#ifndef __SCENE_FRAME_H__
#define __SCENE_FRAME_H__

#include<vector>

struct ScalePixel {
	int startFrameX, startFrameY, widthFrame, heightFrame, wBias, hBias, hBuffer, wBuffer;
};

struct ScaleCell {
	int startFrameX, startFrameY, widthFrame, heightFrame, wBias, hBias;
};

class SceneFrame
{
private:
	int width, height, wCell, hCell;
	double wCountCells, hCountCells;
	ScalePixel pixel;
	ScaleCell cell;

	std::vector<std::pair<int, int>> coordinates;
public:
	SceneFrame();
	~SceneFrame();
	void initialize(int w, int h, int ww, int hw);

	std::vector<std::pair<int, int>> getCoordinates();
	void deleteCoordElements();

	friend class Maze;
	friend class GameScene;
};

#endif

