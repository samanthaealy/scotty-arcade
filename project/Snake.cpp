#include "Snake.h"

void setRandomNumberSeedFromTime(void)
// code borrowed from class
{
	srand(time(NULL));
}

int randomInRange(int n1, int n2)
{
	int r = rand();
	r = (r % (n2)) + n1;
	return r;
}

void Snake::newFood()
{
	int pos = randomInRange(0, wid * hei - 1);
	while (board[pos] != 0) {
		pos = randomInRange(0, wid * hei - 1);
	}
	board[pos] = 5;
}

void Snake::update()
{
	int newHead = getNewPos(head, board[head]);
	if ((newHead == -1 && !validPosition(newHead)) || (board[newHead] > 0 && board[newHead] < 5)) {
		over = 1;
		return;
	}
	if (board[newHead] != 5) {
		int newtail = getNewPos(tail, board[tail]);
		board[tail] = 0;
		tail = newtail;
	}
	else {
		newFood();
		length++;
	}
	board[newHead] = board[head];
	head = newHead;
}

void Snake::cleanup()
{
    wid = 0;
    hei = 0;
    length = 0;
    paused = 0;
    over = 0;
    lastTime = 0;
    if (board != nullptr) {
        delete[] board;
        board = nullptr;
    }
}


int Snake::getNewPos(int pos, int dir) const
{
	int x = pos % wid;
	int y = pos / wid;
	x = x + (dir == 3) - (dir == 4);
	y = y - (dir == 1) + (dir == 2);
	if (x < 0 || x >= wid || y < 0 || y >= hei) return -1;
	int newpos = x + y * wid;
	return newpos;
}

int Snake::validPosition(int pos) const
{
	int x = pos % wid;
	int y = pos / wid;
	if (x < 0 || x >= wid || y < 0 || y >= hei || !(board[pos] == 0 || board[pos] == 5)) return 0;
	return 1;
}

Snake::Snake()
{
	wid = 16;
	hei = 16;
	head = 0;
	tail = 0;
	paused = 0;
	over = 0;
	length = 0;
    lastTime = 0;
    board = nullptr;
}

Snake::~Snake()
{
	cleanup();
}

void Snake::Create()
{
    wid = 16;
	hei = 16;
	head = 0;
	tail = 0;
	paused = 0;
	over = 0;
	length = 0;
	lastTime = 0;
    board = new int[wid * hei];
    for (int i = 0; i < wid * hei; i++) {
        board[i] = 0;
    }
	setRandomNumberSeedFromTime();
	tail = 5 + 8 * wid;
	head = 7 + 8 * wid;
	board[tail] = 3;
	board[getNewPos(tail, 3)] = 3;
	board[head] = 3;
	length = 3;
	newFood();
	lastTime = FsSubSecondTimer();
}

void Snake::Play(int key)
{
	
	if (key == FSKEY_P) {
		Pause();
	}
	if (!paused && !over) {
		if (key == FSKEY_UP) {
			board[head] = 1;
		}
		else if (key == FSKEY_DOWN) {
			board[head] = 2;
		}
		else if (key == FSKEY_RIGHT) {
			board[head] = 3;
		}
		else if (key == FSKEY_LEFT) {
			board[head] = 4;
		}
		if (FsSubSecondTimer() - lastTime > 1000 / length) {
			update();
			lastTime = FsSubSecondTimer();
		}
	}
}

void Snake::Pause()
{
	paused = (paused + 1) % 2;
}

void Snake::instructions()
{
}

void Snake::Draw() const
{
	glBegin(GL_LINE_LOOP);
	int x1 = 100;
	int x2 = 700;
	int y1 = 50;
	int y2 = 650;
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
	glColor3d(1, 0, 0);
	glRasterPos2d(256, 376);
	glColor3d(0, 0, 0);
	glRasterPos2d(320.0, 730.0);
	YsGlDrawFontBitmap20x32("Score:");

	const std::string str = std::to_string(length);
	char const *ch = str.c_str();
	glRasterPos2d(460.0, 730.0);
	YsGlDrawFontBitmap20x32(ch);

	for (int i = 0; i < wid; i++) {
		for (int j = 0; j < hei; j++) {
			int val = board[i + j * wid];
			if (val > 0 && val < 5) {
				glColor3d(0, 1, 0);
			}
			else if (val == 5) {
				glColor3d(1, 0, 0);
			}
			else {
				glColor3d(0, 0, 0);
			}
			glBegin(GL_QUADS);
			int x1 = 100 + 600.0 / wid * i;
			int x2 = 100 + 600.0 / wid * (i + 1);
			int y1 = 50 + 600.0 / hei * j;
			int y2 = 50 + 600.0 / hei * (j + 1);
			glVertex2i(x1, y1);
			glVertex2i(x2, y1);
			glVertex2i(x2, y2);
			glVertex2i(x1, y2);
			glEnd();
		}
	}
	if (over) {
		glColor3d(1, 0, 0);
		glRasterPos2d(277, 376);
		YsGlDrawFontBitmap32x48("Game Over");
	}
	else if (paused) {
		glColor3ub(255, 255, 255);
		glRasterPos2d(340, 150);
		YsGlDrawFontBitmap20x28("Paused");
		glColor3ub(255, 255, 255);
		glRasterPos2d(200.0, 300.0);
		YsGlDrawFontBitmap16x20("Hit P pause/unpause");
		glRasterPos2d(200.0, 350.0);
		YsGlDrawFontBitmap16x20("Use the Arrows to Move");
		glRasterPos2d(200.0, 400.0);
		YsGlDrawFontBitmap16x20("Use ESC to Return to Arcade");
	}
}

void Snake::Delete()
{
	cleanup();
}
