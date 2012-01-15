//====================================================
// App.cpp
//====================================================
#include "DragonFireSDK.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include "Shabb.h"

int Mp3Handle;
int LogoImage;
std::map<std::string, int> views;
std::map<std::string, Shabb::View> sviews;
std::vector<int> images; 
//std::vector<Shabb::Tile> tiles;
int edit;
std::string Scene;

int OnRotate(int value) {
	if (value == 1) {
		// prot
		SHABB_SCREEN_WIDTH = 320;
		SHABB_SCREEN_HEIGHT = 480;
	} else if (value == 2) {
		// landscape
		SHABB_SCREEN_WIDTH = 480;
		SHABB_SCREEN_HEIGHT = 320;
	}

	return 0;
}


void CleanViews() {
	std::map<std::string, Shabb::View>::iterator p;

	for(p = sviews.begin(); p != sviews.end(); p++) {
		std::string name = p->first;
		Shabb::View v = p->second;
		v.hide();
		printf("Deleting: %s\n", name.c_str());
	}
	//-- Todo Erase this map?
	sviews.clear();
}

Shabb::View idToShabbView(int id) {
	std::map<std::string, Shabb::View>::iterator p;

	for(p = sviews.begin(); p != sviews.end(); p++) {
		std::string name = p->first;
		Shabb::View v = p->second;
		if (v.getViewId() == id) {
			return v;
		}
	}

}

int GoToEditor(int id, int event, int x, int y) {
	if (event == 3) {
		Scene = "LoadEditorScene";
	}
	return 0;
}

int GoToGame(int id, int event, int x, int y) {
	if (event == 3) {
		Scene = "LoadGameScene";
		printf("Touch");
	}
	return 0;
}

int GoToGameSimple(int id) {
	GoToGame(id, 3, 0, 0);
	return 0;
}

int ExitGame(int id, int event, int x, int y) {
	if (event == 3) {
		Scene = "Exit";
		exit (1);
	}
	return 0;
}

int LoadMainScene() {
	LogoImage=ImageAdd("Images/Logo.png");
	images.push_back(LogoImage);
	sviews["LogoView"] = Shabb::View::createFromImage(LogoImage, 90, 10);

	
	int playbtn1 = ImageAdd("Images/playbtn1.png");
	images.push_back(playbtn1);
	sviews["PlayButton"] = Shabb::View::createFromImage(playbtn1, 0, 80);
	sviews["PlayButton"].setTouch(GoToGame);

	sviews["EditButton"] = Shabb::View::createFromImage(playbtn1, 0, 150);
	sviews["EditButton"].setTouch(GoToEditor);
	
	int exitbtn1 = ImageAdd("Images/exitbtn1.png");
	images.push_back(exitbtn1);
	sviews["ExitButton"] = Shabb::View::createFromImage(exitbtn1, 0, 220);
	sviews["ExitButton"].setTouch(ExitGame);

	Mp3Handle = Mp3Add("loop-01.mp3");
	Mp3Loop(Mp3Handle);

	return 0;
}

int MainScene() {
	// Center the view :)
	sviews["PlayButton"].center(SHABB_VIEW_X);
	sviews["ExitButton"].center(SHABB_VIEW_X);
	sviews["LogoView"].center(SHABB_VIEW_X);
	return 0;
}

int LoadEditorScene() {
	LogoImage=ImageAdd("Images/map-editor.png");
	images.push_back(LogoImage);
	sviews["LogoView"] = Shabb::View::createFromImage(LogoImage, 0, 0);

	return 0;
}

int EditorScene() {
	return 0;
}

std::vector< std::vector<int> > touches;
int global_x = 0;
int global_y = 0;
bool global_scrolling = false;
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

std::string trim(std::string data) {
    std::string result(data);
    int i,j,start,end;

    //ltrim
    for (i=0; (result[i] != 0 && result[i] <= 32);) {
        i++;
    }
    start = i;

    //rtrim
    for (i = 0,j = 0; result[i] != 0; i++) {
        j = ((result[i] <= 32) ? j + 1 : 0);
    }
    end = i - j;
    result = result.substr(start, end - start);
    
    return result;
}

void AppMain() {
	unsigned short map[255];
	char mapStr[255 * 3];

	for (short i = 0; i < 255; i++) {
		map[i] = 0;
	}
	map[0] = 97;
	map[1] = 98;
	map[2] = 32767;
	map[3] = 999;

	for (unsigned short i = 0; i < 255; i++) {
		std::string tileString;

		if (map[i] == 0) {
			for (int d = 0; d < 3; d++) {
				//-- Save to map
				mapStr[(i * 3) + d] = -1;
			}
		} else {
			//-- Convert tile id to a string
			std::stringstream out;
			out << map[i];
			tileString = out.str();

			//-- If I don't resize to a fixed size, then substr will fall out of range
			tileString.resize(6);

			//-- Split the string number into trhee parts, so they each fit in a char
			int digits[3];
			for (int d = 0; d < 3; d++) {
				if((std::istringstream (tileString.substr(d * 2, 2).c_str()) >> digits[d]).fail()) {
					digits[d] = -1;
				}
				
				//-- Save to map
				printf("index: %d\n", (i * 3) + d);
				mapStr[(i * 3) + d] = digits[d];
			}

			
			printf("numbers: %s %s %s\n", tileString.substr(0, 2).c_str(), tileString.substr(2, 2).c_str(), tileString.substr(4, 2).c_str());
			printf("before: %d %d %d\n", digits[0], digits[1], digits[2]);
			printf("\n");
		}

	}
	int fp = FileCreate("map.txt");
	FileWrite(fp, mapStr, sizeof(map) * 2);
	//FileWrite(fp, "h", 1);
	FileClose(fp);

	Scene = "LoadMainScene";
	AutoRotateInit(3, OnRotate);
	LandscapeMode();
	TimerGetMsExpired();
}

void AppExit()
{
	// Application exit code goes here.  Perform any clean up your app might 
	// need to do in the event of interruption by a phone call or the user
	// pressing the Home button, or some other event that would cause your
	// application to terminate.
	Mp3Stop();
}

void OnTimer()
{
	unsigned int ms = TimerGetMsExpired();

	if (Scene == "LoadMainScene") {
		CleanViews();
		LoadMainScene();
		Scene = "MainScene";
	} else if (Scene == "MainScene") {
		MainScene();
	} else if (Scene == "LoadGameScene") {
		CleanViews();
		Scene = "GameScene";
		LoadGameScene();
	} else if (Scene == "GameScene") {
		GameScene();
	} else if (Scene == "LoadEditorScene") {
		CleanViews();
		Scene = "EditorScene";
		LoadEditorScene();
	} else if (Scene == "EditorScene") {
		EditorScene();
	} else if (Scene == "Exit") {
		CleanViews();
	}
    // Main loop code goes here.  OnTimer() is called 30 times per second.
	//printf("Tick %d\n", ms);
}