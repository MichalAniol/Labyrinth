USING_NS_CC;

float Labyrinth::proportion(int axis = 1)
{
	auto checker = Director::getInstance();
	auto window_size = checker->getWinSize();

	if (axis) return window_size.width / 1024;
	else { return window_size.height / 600; };
}

void Labyrinth::walls()
{
	all_walls = Layer::create();
	all_walls->setAnchorPoint(Vec2::ZERO);
	addChild(all_walls, 3);

	int axis_x = 99;
	for (int x = 0; x < 22; x++)
	{
		int axis_y = -235;
		for (int y = 0; y < 21; y++)
		{
			auto move_single_wall = MoveBy::create(2 + (x + y) * .12f, Point(-12, 0));
			auto scale_single_wall = ScaleTo::create(2 + (x + y) * .12f, looking_for_passage(3, y, x)); // down

			wall = Sprite::create("komponenty/sciana_pozioma.jpg");
			wall->setPosition(axis_x, axis_y);
			wall->setScaleX(0);
			all_walls->addChild(wall, 3);

			wall->runAction(move_single_wall);
			wall->runAction(scale_single_wall);

			axis_y -= 30;
		}
		axis_x += 30;
	}

	axis_x = 102;
	for (int x = 0; x < 21; x++)
	{
		int axis_y = -233;
		for (int y = 0; y < 22; y++)
		{
			auto move_single_wall = MoveBy::create(2 + (x + y) * .12f, Point(0, 12));
			auto scale_single_wall = ScaleTo::create(2 + (x + y) * .12f, looking_for_passage(2, y, x)); //right

			wall = Sprite::create("komponenty/sciana_pionowa.jpg");
			wall->setPosition(axis_x, axis_y);
			wall->setScaleY(0);
			all_walls->addChild(wall, 3);

			wall->runAction(move_single_wall);
			wall->runAction(scale_single_wall);

			axis_y -= 30;
		}
		axis_x += 30;
	}

	int  axis__y = 22, chamber_size_at_pixels = 30;
	int start_position_x = 87 - 2 * chamber_size_at_pixels - chamber_size_at_pixels / 2;
	int start_position_y = 480 - (axis__y + 2) * chamber_size_at_pixels + chamber_size_at_pixels / 2;
	auto showup_map = MoveBy::create(.9f, Point(0, 700));

	empty_map = Sprite::create("komponenty/mpa_cala.jpg");
	empty_map->setAnchorPoint(Vec2::ZERO);
	empty_map->setPosition(start_position_x, start_position_y - 700);
	all_walls->addChild(empty_map, 2);


	auto walls_showup = MoveTo::create(.9f, Point(0, 700));
	all_walls->runAction(walls_showup);
}

bool Labyrinth::bottons_showup()
{
	button_up = Layer::create();
	button_down = Layer::create();
	button_left = Layer::create();
	button_right = Layer::create();

	button_up->setAnchorPoint(Vec2::ZERO);
	button_down->setAnchorPoint(Vec2::ZERO);
	button_left->setAnchorPoint(Vec2::ZERO);
	button_right->setAnchorPoint(Vec2::ZERO);

	button_up->setPosition(Vec2::ZERO);
	button_down->setPosition(Vec2::ZERO);
	button_left->setPosition(Vec2::ZERO);
	button_right->setPosition(Vec2::ZERO);

	addChild(button_up, 6);
	addChild(button_down, 6);
	addChild(button_left, 6);
	addChild(button_right, 6);

	for (int i = 0 ; i < 2; i++)
	{
		std::string which_key;
		switch (i)
		{
			case 0: which_key = "komponenty/klawisz red.png";	break;
			case 1: which_key = "komponenty/klawisz.png";		break;
		}
		key_up = Sprite::create(which_key);	key_up->setPosition(Vec2::ZERO); 
		key_up->setScale(.66f); button_up->addChild(key_up, 6);
		
		key_down = Sprite::create(which_key); key_down->setPosition(Vec2::ZERO); 
		key_down->setScale(.66f); button_down->addChild(key_down, 6);
		
		key_left = Sprite::create(which_key); key_left->setPosition(Vec2::ZERO); 
		key_left->setScale(.66f); button_left->addChild(key_left, 6);

		key_right = Sprite::create(which_key); key_right->setPosition(Vec2::ZERO); 
		key_right->setScale(.66f); button_right->addChild(key_right, 6);
	}

	button_up->setScale(0);
	button_down->setScale(0);
	button_left->setScale(0);
	button_right->setScale(0);

	button_up->setRotation(45-90);
	button_down->setRotation(225 - 90);
	button_left->setRotation(315 - 90);;
	button_right->setRotation(135 - 90);

	button_up->setPosition(884, 620);
	button_down->setPosition(884, 364-60-60-4);
	button_left->setPosition(756-2, 492-60-2);
	button_right->setPosition(1012+2, 492-60-2);

		int x = 0, y = -20;
		auto button_up_move_1 = MoveBy::create(1.2f, Point(x, y));
		auto button_up_move_2 = MoveBy::create(.6f, Point(x * 5, y * 5));
		button_up->runAction(button_up_move_1); button_up->runAction(button_up_move_2);

		x = 0; y = 20;
		auto button_down_move_1 = MoveBy::create(1.2f, Point(x, y));
		auto button_down_move_2 = MoveBy::create(.6f, Point(x * 5, y * 5));
		button_down->runAction(button_down_move_1); button_down->runAction(button_down_move_2);

		x = 10; y = 0;
		auto button_left_move_1 = MoveBy::create(1.2f, Point(x, y));
		auto button_left_move_2 = MoveBy::create(.6f, Point(x * 5, y * 5));
		button_left->runAction(button_left_move_1); button_left->runAction(button_left_move_2);

		x = -10; y = 0;
		auto button_right_move_1 = MoveBy::create(1.2f, Point(x, y));
		auto button_right_move_2 = MoveBy::create(.6f, Point(x * 5, y * 5));
		button_right->runAction(button_right_move_1); button_right->runAction(button_right_move_2);


	for (int i = 0; i < 4; i++)
	{
		auto buttons_scale = ScaleTo::create(.3f, 1);
		auto buttons_rotation = RotateBy::create(.3f, 90);
		auto sequ = Sequence::create(buttons_scale, buttons_rotation, nullptr);

		switch (i)
		{
		case 0: button_up->runAction(sequ); break;
		case 1: button_down->runAction(sequ); break;
		case 2: button_left->runAction(sequ); break;
		case 3: button_right->runAction(sequ); break;
		}
	}

	button_reset = Sprite::create("komponenty/restart.png");
	button_reset->setPosition(884, 90);
	button_reset->setScale(.667f);
	addChild(button_reset, 6);
	
	key_up->setScale(.1f);
	key_down->setScale(.1f);
	key_left->setScale(.1f);
	key_right->setScale(.1f);

	auto key_up_regular_scale = ScaleTo::create(.5f, .69f);
	auto key_down_regular_scale = ScaleTo::create(.5f, .69f);
	auto key_left_regular_scale = ScaleTo::create(.5f, .69f);
	auto key_right_regular_scale = ScaleTo::create(.5f, .69f);

	key_up->runAction(key_up_regular_scale);
	key_down->runAction(key_down_regular_scale);
	key_left->runAction(key_left_regular_scale);
	key_right->runAction(key_right_regular_scale);


	return true;
}

bool Labyrinth::ball_on_start()
{
	auto pul_ball_at_map = MoveTo::create(.9f, Point(87, 480));
	auto drop_ball_at_map = ScaleTo::create(1.1f, .12f);

	ball = Sprite::create("komponenty/pilkad.png");
	ball->setPosition(87, 630);
	addChild(ball, 7);
	ball->runAction(pul_ball_at_map);
	ball->runAction(drop_ball_at_map);

	return true;
}

bool Labyrinth::touch_surfaces()
{
	for (int i = 0; i < 4; i++)
	{
		int x, y;
		switch (i)
		{
		case 0: x = 815, y = 431; break;
		case 1: x = 815, y = 291; break;
		case 2: x = 745, y = 361; break;
		case 3: x = 885, y = 361; break;
		}
		auto touch_surface = DrawNode::create();
		{Point Block1[] = { Point(0, 103) ,Point(103, 0) ,Point(103 * 2, 103),Point(103, 103 * 2), };
		touch_surface->drawPolygon(Block1, 4, Color4F(0, 100, 0, 128), 1, Color4F(0, 0, 0, 0)); }

		touch_surface->setAnchorPoint(Vec2::ZERO);
		touch_surface->setScale(.66f);
		touch_surface->setPosition(x, y);
		addChild(touch_surface, 7);
	}
	return true;
}

void  Labyrinth::the_win()
{
	int move_a_bit = 100;

	cup = Sprite::create("komponenty/cup.png");
	cup->setPosition(512 - move_a_bit, 300);
	cup->setScaleY(0);
	cup->setScaleX(.6f);
	auto finmove = ScaleTo::create(.6f, 1);
	addChild(cup, 8);
	cup->runAction(EaseElasticOut::create(finmove));

	finito1 = Sprite::create("komponenty/congratulations.png");
	finito1->setPosition(512 - move_a_bit, 750);
	auto fin1move = MoveTo::create(1.2f, Point(512 - move_a_bit, 460));
	addChild(finito1, 7);
	finito1->runAction(EaseBounceOut::create(fin1move));

	finito2 = Sprite::create("komponenty/you won.png");
	finito2->setPosition(512 - move_a_bit, -150);
	auto fin2move = MoveTo::create(1.2f, Point(512 - move_a_bit, 120));
	addChild(finito2, 7);
	finito2->runAction(EaseBounceOut::create(fin2move));

	glitter = Sprite::create("komponenty/glitter.png");
	glitter->setScale(0);
	glitter->setPosition(500 - move_a_bit, 320);
	auto shine = ScaleTo::create(.9f, .5f);
	auto sparkle = RotateBy::create(20, 360);
	auto never_stop = RepeatForever::create(sparkle);
	addChild(glitter, 6);
	glitter->runAction(shine);
	glitter->runAction(never_stop);
}
