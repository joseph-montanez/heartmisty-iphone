int TouchedTile(int id, int event, int x, int y) {
	std::vector<int> point;
	point.push_back(x);
	point.push_back(y);
	touches.push_back(point);
	printf("x: %d ", x);
	printf("y: %d\n", y);

	//-- Test for possible swipping?
	if (touches.size() > 15) {
		//-- 15 touch points is 1/2 of a second.
		int diff;
		std::vector<int> p1, p2;
		p1 = touches[0];
		p2 = touches[touches.size() - 1];
		//-- See if the first and last touch points are small enough to consider scrolling. 
		if (p1[1] - p2[1] < 15) {
			printf("scroll!\n");
			global_x += (p1[0] - p2[0]) / 20;
			if (global_x < 0) {
				global_x = 0;
			}
			else if (global_x > 32 * 50) {
				global_x = 32 * 50;
			}
			printf("Global X: %d\n", global_x);
			WorldSetxy(global_x, 0);
			global_scrolling = true;
		}
		touches.clear();
		/*
		for (unsigned int i = 0; i < touches.size(); i++) {
			point = touches[i];
			printf("x: %d ", point[0]);
			printf("y: %d\n", point[1]);
		}
		*/
	}
	else if (global_scrolling == true && touches.size() > 1) {
		int diff;
		std::vector<int> p1, p2;
		p1 = touches[0];
		p2 = touches[touches.size() - 1];

		printf("scroll!\n");
		global_x += (p1[0] - p2[0]) * 2;
		if (global_x < 0) {
			global_x = 0;
		}
		else if (global_x > 32 * 50) {
			global_x = 32 * 50;
		}
		printf("Global X: %d\n", global_x);
		WorldSetxy(global_x, 0);
		global_scrolling = 1;
		
		touches.clear();

	}
	if (event == 3) {
		printf("%d touches\n", touches.size());
		printf("%d Touched on %d x %d\n", id, x, y);
		touches.clear();
		global_scrolling = 0;
		
	}
	return 0;
}

int LoadGameScene() {
	Mp3Stop();
	/*
	LogoImage=ImageAdd("Images/button1.png");
	images.push_back(LogoImage);
	sviews["Button"] = Shabb::View::createFromImage(LogoImage, 90, 50);
	*/
	
	int grass = ImageAdd("Images/grass.png");
	images.push_back(grass);
	for (int x = 0; x < 50; x++) {
		for (int y = 0; y < 10; y++) {
			char buffer[16];
			sprintf (buffer, "%dx%d", x, y);
			sviews[buffer] = Shabb::View::createFromImage(grass, x * 32, y * 32);
			sviews[buffer].setTouch(TouchedTile);
		}
	}
	return 0;
}

int GameScene() {
	//sviews["Button"].addY(1);
	return 0;
}