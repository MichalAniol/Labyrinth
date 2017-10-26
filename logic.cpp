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


int Labyrinth::looking_for_the_way_out()
{

	int look_at_y = 0, look_at_x = 0, chamber_counter = 0;
	int map[22][22];
	int *adress_way = &way_out[0][0], *adres_map = &map[0][0], *adres_board = &board[0][0];
	int sum_of_exits, chamber;

	for (int i = 0; i < 300; i++) *adress_way++ = 0;						//cleanig of the way
	for (int i = 0; i < 22 * 22; i++) *adres_map++ = *adres_board++;        //copy of the labyrinth

	do
	{

		chamber = map[look_at_y][look_at_x];
		sum_of_exits = (chamber & 1) + (chamber & 2) / 2 + (chamber & 4) / 4 + (chamber & 8) / 8;

		if (sum_of_exits == 0)             // if it's death end go back to last cros
		{
			do {
				chamber_counter--;
				look_at_y = way_out[chamber_counter][0];
				look_at_x = way_out[chamber_counter][1];
			} while (map[look_at_y][look_at_x] == 0); continue;
		}

		way_out[chamber_counter][0] = look_at_y;               // note localisation at the way
		way_out[chamber_counter][1] = look_at_x;


		if (chamber & 2) { map[look_at_y][look_at_x] -= 2; look_at_x++; map[look_at_y][look_at_x] -= 8; chamber_counter++; continue; }

		if (chamber & 4) { map[look_at_y][look_at_x] -= 4; look_at_y++; map[look_at_y][look_at_x] -= 1; chamber_counter++; continue; }

		if (chamber & 1) { map[look_at_y][look_at_x] --  ; look_at_y--; map[look_at_y][look_at_x] -= 4; chamber_counter++; continue; }

		if (chamber & 8) { map[look_at_y][look_at_x] -= 8; look_at_x--; map[look_at_y][look_at_x] -= 2; chamber_counter++; continue; }

	} while (!(look_at_y == 21 && look_at_x == 21));

	way_out[chamber_counter][0] = look_at_y;        // exit of labytrinth
	way_out[chamber_counter][1] = look_at_x;

	return ++chamber_counter;	// how logn the way is
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
	auto push_the_buton3 = ScaleTo::create(2.1f, 1 * .667f);
	auto push_the_buton4 = ScaleTo::create(.1f, .9f * .667f);


	auto move_ball_up1 = MoveBy::create(.1f, Point(0, 30));
	auto move_ball_up2 = MoveBy::create(.1f, Point(0, 2));
	auto move_all_walls_up = MoveBy::create(.3f, Point(0, -28));
	auto move_the_way_out_up = MoveBy::create(.3f, Point(0, -28));
	auto move_ball_down1 = MoveBy::create(.1f, Point(0, -30));
	auto move_ball_down2 = MoveBy::create(.1f, Point(0, -2));
	auto move_all_walls_down = MoveBy::create(.3f, Point(0, 28));
	auto move_the_way_out_down = MoveBy::create(.3f, Point(0, 28));
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
					ariadnes_thread->runAction(move_the_way_out_up);
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
					ariadnes_thread->runAction(move_the_way_out_down);
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

	// scene show me the way out
	if (button_way_out->getBoundingBox().containsPoint(screen_touch))
	{
		button_way_out->runAction(push_the_buton3);
		button_way_out->runAction(push_the_buton4);

		if (ariadnes_thread->getOpacity())
		{
			auto visibility = FadeTo::create(2, 0);
			ariadnes_thread->runAction(visibility);
		}
		else
		{
			auto visibility = FadeTo::create(2, 175);
			ariadnes_thread->runAction(visibility);
		}

	}


	return true;
}

int Labyrinth::surface_is_touched(int y, int x)
{
	if (((0 < x && x < 70) && (67 - x < y && y < 69 + x)) || ((69 < x && x < 138) && (x - 69 < y && y < 138 + 68 - x)))
		return true;
	else return false;
}