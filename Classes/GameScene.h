#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "Maze.h"
#include "SpriteSheetsName.h"
#include "SceneFrame.h"

#include<cmath>
#include<vector>
#include<algorithm>

static const float DELAY = 0.03;

class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(GameScene);

	void menuCloseCallback(cocos2d::Ref* pSender);
	virtual void update(float delta);

	GameScene();
	~GameScene();

private:
	cocos2d::Layer* layer2;
	int speed = 300, stop_action_tag = 1;
	int widthWall, heightWall, rightXposition = 0, rightYposition = 0;

	Maze maze;
	SceneFrame sceneFrame;

	cocos2d::SpriteFrame * wallFrame;
	cocos2d::Sprite * survivor;

	cocos2d::Vector<cocos2d::SpriteFrame*> curentAnimationIdle;
	cocos2d::Vector<cocos2d::SpriteFrame*> curentAnimationMove;
	cocos2d::Vector<cocos2d::SpriteFrame*> curentAnimationAttack;
	std::vector<cocos2d::EventKeyboard::KeyCode> keyCodes;
	std::vector<cocos2d::Sprite*> wallSprites;


	cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char *format, int count);

	void keyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event);
	void mouseMove(cocos2d::Event *event);
	void mouseDown(cocos2d::Event *event);
	void updateWalls();
};

#endif

