USING_NS_CC;

void Labyrinth::board_generator(int * board, int board_size_x, int board_size_y)
{
	/*********************** LABYRINTH GENERATOR **************************/

	int chamber_axis_x, chamber_axis_y; 
	int meter_of_empty_chambers = (board_size_x * board_size_y) - 1;

	int direction = 0;				// nibbel %0001 == open the top wall
									// nibbel %0010 == open the right wall
									// nibbel %0100 == open the bottom wall
									// nibbel %1000 == open the left wall

	int empty[4] = { 0,0,0,0 };		// holder of possibilities to open next chamber


	srand(time(NULL));											// looking for first chamber
	chamber_axis_x = (rand() % (board_size_x - 2)) + 1;			// do not start form borders
	chamber_axis_y = (rand() % (board_size_y - 2)) + 1;			// especially do not start from corners

	do {
		do {
			direction = 0;

			if (chamber_axis_x                     && *(board + (board_size_y * chamber_axis_y)   + chamber_axis_x - 1) == 0) empty[direction++] = 8; // left
			if (chamber_axis_x != board_size_x - 1 && *(board + (board_size_y * chamber_axis_y)   + chamber_axis_x + 1) == 0) empty[direction++] = 2; // right
			if (chamber_axis_y                     && *(board + (board_size_y * (chamber_axis_y - 1)) + chamber_axis_x) == 0) empty[direction++] = 1; // up
			if (chamber_axis_y != board_size_y - 1 && *(board + (board_size_y * (chamber_axis_y + 1)) + chamber_axis_x) == 0) empty[direction++] = 4; // down

			if (direction)
			{
				int chose_which_one_of_the_chambers_wiil_be_open = rand() % (direction);
				int open = empty[chose_which_one_of_the_chambers_wiil_be_open];

				*(board + (board_size_y * chamber_axis_y) + chamber_axis_x) += open; // open chamber where you are

				if (open == 8) { chamber_axis_x--; *(board + (board_size_y * chamber_axis_y) + chamber_axis_x) = 2; }; // open chaber
				if (open == 2) { chamber_axis_x++; *(board + (board_size_y * chamber_axis_y) + chamber_axis_x) = 8; }; // where you go
				if (open == 1) { chamber_axis_y--; *(board + (board_size_y * chamber_axis_y) + chamber_axis_x) = 4; };
				if (open == 4) { chamber_axis_y++; *(board + (board_size_y * chamber_axis_y) + chamber_axis_x) = 1; };

				meter_of_empty_chambers--;
			}
		} while (direction);  // dead end

		do {	// find a chamber whit passage, and go to check if it has emty neighbor
			chamber_axis_x++; if (chamber_axis_x == board_size_x)
			{
				chamber_axis_x = 0; chamber_axis_y++;
				if (chamber_axis_y == board_size_y)
				{
					chamber_axis_y = 0;
				};
			}
		} while (!*(board + (board_size_y * chamber_axis_y) + chamber_axis_x));

	} while (meter_of_empty_chambers > 0); // do it as long every chamber has passage
}

int Labyrinth::looking_for_passage(int direction, int here_i_am_y, int here_i_am_x)
{
	int chamber_im_on = board[here_i_am_y][here_i_am_x];
	int  rezultat;
	switch (direction)
	{
	case 1: if (chamber_im_on & 1) rezultat = 0; else rezultat = 1; break; // %0001 - up
	case 2: if (chamber_im_on & 2) rezultat = 0; else rezultat = 1; break; // %0010 - right
	case 3: if (chamber_im_on & 4) rezultat = 0; else rezultat = 1; break; // %0100 - down
	case 4: if (chamber_im_on & 8) rezultat = 0; else rezultat = 1; break; // %1000 - left
	}
	return rezultat;
}

void Labyrinth::moves()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Labyrinth::Toucher, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool Labyrinth::Toucher(cocos2d::Touch *touch, cocos2d::Event *event)
{
	auto ball_position_y = ball->getPosition().y;
	auto board_positon_y = all_walls->getPosition().y;
	int touch_x = touch->getLocation().x;
	int touch_y = touch->getLocation().y;

	auto push_the_buton1 = ScaleTo::create(2.1f, 1);
	auto push_the_buton2 = ScaleTo::create(.1f, .9f);
	auto red_button1 = FadeTo::create(.6f, 255);
	auto red_button2 = FadeTo::create(.2f, 0);

	auto move_ball_up1 = MoveBy::create(.1f, Point(0, 30));
	auto move_ball_up2 = MoveBy::create(.1f, Point(0, 2));
	auto move_all_walls_up = MoveBy::create(.3f, Point(0, -28));
	auto move_ball_down1 = MoveBy::create(.1f, Point(0, -30));
	auto move_ball_down2 = MoveBy::create(.1f, Point(0, -2));
	auto move_all_walls_down = MoveBy::create(.3f, Point(0, 28));
	auto move_ball_left = MoveBy::create(.1f, Point(-30, 0));
	auto move_ball_right = MoveBy::create(.1f, Point(30, 0));

	if (!win)
	{
		int x = touch_x / proportion(true) - 815, y = touch_y / proportion(false) - 431; // go up
		if (surface_is_touched(y, x))
		{
			if (!looking_for_passage(1, here_i_am_y, here_i_am_x))
			{
				if (ball_position_y > 461 && board_positon_y > 720)
				{
					ball->runAction(move_ball_up2);
					all_walls->runAction(move_all_walls_up);
				}
				else ball->runAction(move_ball_up1);
				button_up->runAction(push_the_buton1);
				button_up->runAction(push_the_buton2);
				here_i_am_y--;
			}
			else
			{
				key_up->runAction(red_button1);
				key_up->runAction(red_button2);
			}
		}


		x = touch_x / proportion(true) - 815, y = touch_y / proportion(false) - 291; // go down
		if (surface_is_touched(y, x))
		{
			if (!looking_for_passage(3, here_i_am_y, here_i_am_x))
			{
				if (ball_position_y < 141 && board_positon_y < 930)
				{
					ball->runAction(move_ball_down2);
					all_walls->runAction(move_all_walls_down);
				}
				else ball->runAction(move_ball_down1);
				button_down->runAction(push_the_buton1);
				button_down->runAction(push_the_buton2);
				here_i_am_y++;
			}
			else
			{
				key_down->runAction(red_button1);
				key_down->runAction(red_button2);
			}
		}

		x = touch_x / proportion(true) - 745, y = touch_y / proportion(false) - 361; // go left
		if (surface_is_touched(y, x))
		{
			if (!looking_for_passage(4, here_i_am_y, here_i_am_x))
			{
				ball->runAction(move_ball_left);
				button_left->runAction(push_the_buton1);
				button_left->runAction(push_the_buton2);
				here_i_am_x--;
			}
			else
			{
				key_left->runAction(red_button1);
				key_left->runAction(red_button2);
			}
		}

		x = touch_x / proportion(true) - 885, y = touch_y / proportion(false) - 361; // go right
		if (surface_is_touched(y, x))
		{
			if (!looking_for_passage(2, here_i_am_y, here_i_am_x))
			{
				ball->runAction(move_ball_right);
				button_right->runAction(push_the_buton1);
				button_right->runAction(push_the_buton2);
				here_i_am_x++;
			}
			else
			{
				key_right->runAction(red_button1);
				key_right->runAction(red_button2);
			}
		}

		if (here_i_am_x == 21 && here_i_am_y == 21)
		{
			the_win();
			win = true;
			auto bigerr_restert = ScaleTo::create(5, 1);
			button_reset->runAction(bigerr_restert);
		}
	}

	auto screen_touch = touch->getLocation(); // scene restart
	if (button_reset->getBoundingBox().containsPoint(screen_touch))
	{
		button_reset->runAction(push_the_buton1);
		button_reset->runAction(push_the_buton2);
		here_i_am_y = 0, here_i_am_x = 0;

		auto newScene = Labyrinth::createScene();
		Director::getInstance()->replaceScene(newScene);
	}

	return true;

}

int Labyrinth::surface_is_touched(int y, int x)
{
	if (((0 < x && x < 70) && (67 - x < y && y < 69 + x)) || ((69 < x && x < 138) && (x - 69 < y && y < 138 + 68 - x)))
		return true;
	else return false;
}