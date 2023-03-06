void windowResize(int width, int height);

void refreshDisplay();

void onMouseWheel([[maybe_unused]] int button, int dir, int x, int y);

void onKeyDown(unsigned char character, int x, int y);

void drawGrid();

void drawAxis();

void bitmapString(double x, double y, double mxPx, double myPx, const char *string);
