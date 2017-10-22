#ifndef __Labyrinth_h__
#define __Labyrinth_h__

#include "cocos2d.h"
#include <time.h>


class Labyrinth : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(Labyrinth);


private:
	int board[22][22];
	int here_i_am_x, here_i_am_y;
	int win = false;

	void board_generator(int * board, int board_size_x, int board_size_y);
	int looking_for_passage(int direction, int here_i_am_x, int here_i_am_y);
	void moves();
	bool Toucher(cocos2d::Touch *touch, cocos2d::Event *event);
	int surface_is_touched(int y, int x);

	float proportion(int axis);
	void walls();
	bool bottons_showup();
	bool ball_on_start();
	bool touch_surfaces();
	void  the_win();

	cocos2d::Sprite* empty_map;
	cocos2d::Sprite* wall;
	cocos2d::Layer* all_walls;

	cocos2d::Sprite* key_up;
	cocos2d::Sprite* key_down;
	cocos2d::Sprite* key_right;
	cocos2d::Sprite* key_left;
	cocos2d::Sprite* button_reset;

	cocos2d::Layer* button_up;
	cocos2d::Layer* button_down;
	cocos2d::Layer* button_right;
	cocos2d::Layer* button_left;

	cocos2d::Sprite* ball;

	cocos2d::Sprite* cup;
	cocos2d::Sprite* finito1;
	cocos2d::Sprite* finito2;
	cocos2d::Sprite* glitter;
};

#endif // __Labyrinth_h__
