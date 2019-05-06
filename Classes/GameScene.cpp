#include "GameScene.h"

USING_NS_CC;


GameScene::GameScene()
{
	maze.generateMaze();
}

GameScene::~GameScene()
{
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(PLIST);

	auto backgroundFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BACKGROUND);

	Sprite* background = Sprite::createWithSpriteFrame(backgroundFrame);
	//float scale = MAX(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
	//background->setScale(scale);
	
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	auto layer = Layer::create();
	layer->addChild(background);
	this->addChild(layer, 1);

	wallFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(WALL);
	
	//change with using scale
	widthWall = wallFrame->getOriginalSize().width;
	heightWall = wallFrame->getOriginalSize().height;

	curentAnimationIdle = getAnimation(SURVIVOR_IDLE_KNIFE, SURVIVOR_IDLE_KNIFE_SIZE);
	curentAnimationMove = getAnimation(SURVIVOR_MOVE_KNIFE, SURVIVOR_MOVE_KNIFE_SIZE);
	curentAnimationAttack = getAnimation(SURVIVOR_MELEEATTACK_KNIFE, SURVIVOR_MELEEATTACK_KNIFE_SIZE);

	survivor = Sprite::createWithSpriteFrame(curentAnimationIdle.front());
	survivor->setPosition(350, 250);

	float radius = (survivor->getContentSize().width > survivor->getContentSize().height ?
		survivor->getContentSize().height : survivor->getContentSize().width) / 2;
	PhysicsBody * pb = PhysicsBody::createCircle(radius, PhysicsMaterial(0.0f, 0.0f, 0.0f));
	pb->setGravityEnable(false);
	survivor->addComponent(pb);

	layer2 = Layer::create();
	layer2->addChild(survivor, 2);
	this->addChild(layer2, 2);

	auto keyListener = cocos2d::EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::keyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::keyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	auto mouseListener = cocos2d::EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::mouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::mouseDown, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto followTheSprite = Follow::create(survivor, Rect(0, 0, Maze::WIDTH * widthWall, Maze::HEIGHT * heightWall));
	layer2->runAction(followTheSprite);

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);
	
	sceneFrame.initialize(visibleSize.width, visibleSize.height, widthWall, heightWall);
	maze.updateSceneFrame(sceneFrame, 0, 0);
	updateWalls();

	this->scheduleUpdate();

	return true;
}

cocos2d::Vector<cocos2d::SpriteFrame*> GameScene::getAnimation(const char *format, int count)
{
	auto spritecache = SpriteFrameCache::getInstance();
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100];

	for (int i = 0; i < count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}

	return animFrames;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void GameScene::keyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
	bool b = false;

	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
		if (std::find(keyCodes.begin(), keyCodes.end(), EventKeyboard::KeyCode::KEY_W) == keyCodes.end())
			keyCodes.push_back(EventKeyboard::KeyCode::KEY_W);

		b = true;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		if (std::find(keyCodes.begin(), keyCodes.end(), EventKeyboard::KeyCode::KEY_A) == keyCodes.end())
			keyCodes.push_back(EventKeyboard::KeyCode::KEY_A);

		b = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
		if (std::find(keyCodes.begin(), keyCodes.end(), EventKeyboard::KeyCode::KEY_S) == keyCodes.end())
			keyCodes.push_back(EventKeyboard::KeyCode::KEY_S);

		b = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		if (std::find(keyCodes.begin(), keyCodes.end(), EventKeyboard::KeyCode::KEY_D) == keyCodes.end())
			keyCodes.push_back(EventKeyboard::KeyCode::KEY_D);

		b = true;
		break;
	}

	if (b)
	{
		auto animation = Animation::createWithSpriteFrames(curentAnimationMove, DELAY);
		animation->setRestoreOriginalFrame(true);

		RepeatForever* rf = RepeatForever::create(Animate::create(animation));
		rf->setTag(stop_action_tag);

		survivor->runAction(rf);
	}
}

void GameScene::keyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
	keyCodes.clear();
	survivor->stopActionByTag(stop_action_tag);
}

void GameScene::mouseMove(cocos2d::Event *event)
{
	EventMouse* e = (EventMouse*) event;

	int mx = e->getCursorX() , my = e->getCursorY();

	Vec2 vec = survivor->getPosition();
	vec = layer2->convertToWorldSpace(vec);

	int sx = vec.x, sy = vec.y;

	double x = sx - mx, y = sy - my;

	float p = 180 - std::atan(y / x) * 180.0 / 3.14;

	if (x < 0)
		p = 180 + p;

	survivor->setRotation(p);
}

void GameScene::mouseDown(cocos2d::Event *event)
{
	if (survivor->numberOfRunningActions() != 0)
		return;

	auto animation = Animation::createWithSpriteFrames(curentAnimationAttack, DELAY);

	animation->setRestoreOriginalFrame(true);
		
	survivor->runAction(Animate::create(animation));
}

void GameScene::update(float delta)
{
	int i, bias = speed * delta;
	Vec2 vec = survivor->getPosition();
	Size vs = Director::getInstance()->getVisibleSize();

	for (i = 0; i < keyCodes.size(); i++)
	{
		EventKeyboard::KeyCode code = keyCodes.at(i);
		switch (code)
		{
		case EventKeyboard::KeyCode::KEY_W:
			survivor->setPosition(vec.x, vec.y += bias);
			break;
		case EventKeyboard::KeyCode::KEY_A:
			survivor->setPosition(vec.x -= bias, vec.y);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			survivor->setPosition(vec.x, vec.y -= bias);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			survivor->setPosition(vec.x += bias, vec.y);
			break;
		}
	}

	if (vec.x < sceneFrame.pixel.startFrameX + sceneFrame.pixel.wBias && vec.x > sceneFrame.pixel.wBias ||
		vec.x > sceneFrame.pixel.startFrameX + sceneFrame.pixel.wBuffer ||
		vec.y < sceneFrame.pixel.startFrameY + sceneFrame.pixel.hBias && vec.y > sceneFrame.pixel.hBias
		|| vec.y > sceneFrame.pixel.startFrameY + sceneFrame.pixel.hBuffer)
	{
		maze.updateSceneFrame(sceneFrame, vec.x, vec.y);

		updateWalls();
	}
}

void GameScene::updateWalls()
{
	int i;
	auto it = wallSprites.begin();

	for (it; it != wallSprites.end();)
	{
		Vec2 vec = (*it)->getPosition();

		if (vec.x < sceneFrame.pixel.startFrameX || vec.x > sceneFrame.pixel.startFrameX + sceneFrame.pixel.widthFrame ||
			vec.y < sceneFrame.pixel.startFrameY || vec.y > sceneFrame.pixel.startFrameY + sceneFrame.pixel.heightFrame)
		{
			(*it)->removeFromParent();
			it = wallSprites.erase(it);
		}
		else
		{
			it++;
		}
	}


	std::vector<std::pair<int, int>> coord = sceneFrame.getCoordinates();

	for (i = 0; i < coord.size(); i++)
	{
    	Sprite * sp = Sprite::createWithSpriteFrame(wallFrame->clone());

		sp->setPosition(coord.at(i).first, coord.at(i).second);

		PhysicsBody * phBodyWall = PhysicsBody::createBox(Size(widthWall, heightWall), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		phBodyWall->setDynamic(false);
		sp->setPhysicsBody(phBodyWall);

		layer2->addChild(sp);

		wallSprites.push_back(sp);
	}

	sceneFrame.deleteCoordElements();
}