int TouchedTile(int id, int event, int x, int y) {
	//-- Add a touch point to the touches vector
	std::vector<int> point;
	point.push_back(x);
	point.push_back(y);
	touches.push_back(point);

	//-- Scroll modifiers
	int scrolling_x = 0;
	int scrolling_y = 0;

	//-- Test for possible panning
	if (touches.size() > 15) {
		int diff;
		std::vector<int> p1, p2;

		//-- Get the first and last touch points
		p1 = touches[0];
		p2 = touches[touches.size() - 1];

		//-- See if the first and last touch points are small enough to consider scrolling. 
		if (p1[1] - p2[1] < 15) {
			scrolling_x = (p1[0] - p2[0]) / 20;
			global_scrolling = true;
		}

		//-- Might not be best to clear so I can test for other jesters?
		//-- I.E swips are fast and may need more touch points, will
		//-- panning will require less touch points.
		touches.clear();
	}
	else if (global_scrolling == true && touches.size() > 1) {
		int diff;
		std::vector<int> p1, p2;
		p1 = touches[0];
		p2 = touches[touches.size() - 1];

		scrolling_x += (p1[0] - p2[0]) * 2;
		if (global_x < 0) {
			global_x = 0;
		}
		else if (global_x > 32 * 50) {
			global_x = 32 * 50;
		}
		WorldSetxy(global_x, 0);
		touches.clear();
	}

	if (global_scrolling == true && scrolling_x > 0) {
		global_x += scrolling_x;

		//-- Make sure to never go beyound a possible touch points
		if (global_x < 0) {
			global_x = 0;
		}
		else if (global_x > 32 * 50) {
			global_x = 32 * 50;
		}
		WorldSetxy(global_x, 0);
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