#include "Labyrinth.h"
#include "SimpleAudioEngine.h"

#include "logic.cpp"
#include "interface.cpp"




USING_NS_CC;

Scene* Labyrinth::createScene()
{
	auto checker = Director::getInstance();
	auto window_size = checker->getWinSize();

	auto scena = Labyrinth::create();

	scena->setScaleX(window_size.width / 1024);
	scena->setScaleY(window_size.height / 600);

	scena->setAnchorPoint(Vec2::ZERO);
	scena->setPosition(Vec2::ZERO);
	return scena;
}

bool Labyrinth::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto background = Sprite::create("komponenty/tlo2.jpg");
	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);
	addChild(background, 0);

	win = false;

	bottons_showup();

	ball_on_start();

	board_generator(&board[0][0], 22, 22);

	walls();

	//touch_surfaces();		// turn it on if wanna see active areas

	moves();

	return true;
}


void Labyrinth::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
