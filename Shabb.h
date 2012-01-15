#define SHABB_VIEW_X  0
#define SHABB_VIEW_Y  1
#define SHABB_VIEW_XY 2

int SHABB_SCREEN_WIDTH = 320;
int SHABB_SCREEN_HEIGHT = 480;

namespace Shabb {
	class View {
	public:
		View ();
		static View createFromImage(int image, int x, int y);
		static View createFromFilename(char *filename, int x, int y);
		int getX ();
		int getY ();
		int getWidth();
		int getHeight();
		int getViewId();
		void setAlpha(int alpha);
		void setImage(int image);
		void setSize(int width, int height);
		void setXY(int x, int y);
		void setRotation(int degrees);
		void setTouch(int (*callback)(int id, int event, int x, int y));
		void setVisible(int flag);
		void setViewId(int id);
		void center(int flag);
		void show();
		void hide();
		void addY(int y);
		void addX(int x);
	protected:
		int viewId;
		int imageId;
		int touchId;
		int (*touchFunction)(int id, int event, int x, int y);
	};

	View::View() {
	}

	void View::setViewId(int id) {
		viewId = id;
	}

	View View::createFromImage(int image, int x, int y) {
		View v;
		v.setViewId(ViewAdd(image, x, y));
		return v;
	}

	View View::createFromFilename(char *filename, int x, int y) {
		View v;
		v.setViewId(ViewAdd(filename, x, y));
		return v;
	}

	int View::getViewId() {
		return viewId;
	}

	int View::getX() {
		return ViewGetx(viewId);
	}

	int View::getY() {
		return ViewGety(viewId);
	}

	int View::getHeight() {
		return ViewGetHeight(viewId);
	}

	int View::getWidth() {
		return ViewGetWidth(viewId);
	}

	void View::setAlpha(int a) {
		ViewSetAlpha(viewId, a);
	}

	void View::setImage(int image) {
		imageId = image;
		ViewSetImage(viewId, imageId);
	}

	void View::setRotation(int degrees) {
		ViewSetRotate(viewId, degrees);
	}

	void View::setSize(int width, int height) {
		ViewSetSize(viewId, width, height);
	}

	void View::setXY(int x, int y) {
		ViewSetxy(viewId, x, y);
	}

	void View::setVisible(int flag) {
		ViewSetVisible(viewId, flag);
	}

	void View::hide() {
		setVisible(0);
	}

	void View::show() {
		setVisible(1);
	}

	void View::addY(int y) {
		setXY(getX(), getY() + y);
	}

	void View::addX(int x) {
		setXY(getX() + x, getY());
	}

	void View::setTouch(int (*callback)(int id, int event, int x, int y)) {
		//-- TODO on touch build in a search function
		touchFunction = callback;
		touchId = TouchAdd(0, 0, getWidth(), getHeight(), touchFunction, viewId);
		ViewSetTouch(viewId, touchId);
	}

	void View::center(int flag) {
		int shabb_x;
		int shabb_y;
		if (flag == SHABB_VIEW_X) {
			int shabb_view_width = ViewGetWidth(viewId);
			shabb_y = ViewGety(viewId);
			shabb_x = (SHABB_SCREEN_WIDTH - shabb_view_width) / 2;
		}
		else if (flag == SHABB_VIEW_Y) {
			int shabb_view_height = ViewGetHeight(viewId);
			shabb_y = (SHABB_SCREEN_HEIGHT - shabb_view_height) / 2;
			shabb_x = ViewGetx(viewId); 
		}
		else if (flag == SHABB_VIEW_XY) {
			int shabb_view_width = ViewGetWidth(viewId);
			int shabb_view_height = ViewGetHeight(viewId);
			shabb_y = (SHABB_SCREEN_HEIGHT - shabb_view_height) / 2;
			shabb_x = (SHABB_SCREEN_WIDTH - shabb_view_width) / 2;
		}
		ViewSetxy(viewId, shabb_x, shabb_y);
	}
	/*
	class Tile {
	public:
		Tile (char* data);
	protected:
		int x;
		int y;
		std::string name;
	};

	Tile::Tile (char* data) {
		x = 0;
		y = 0;
		name = "";
	}
	*/
}