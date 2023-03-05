void windowResize(int width, int height);

void render();

void onMouseWheel(int button, int dir, int x, int y);

void drawGrid();

void drawAxis();

void pointsToScreenVertices(double x1, double y1, double x2, double y2);

void pointToScreen(double x, double y, double &screenX, double &screenY);

void bitmapString(double x, double y, const char *string);

void drawGraph(double (*func)(double));
