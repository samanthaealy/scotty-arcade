#include "fssimplewindow.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "ysglfontdata.h"
#include "Home Screen.h"

class Snake : public Game
{
private:
	int *board;
	int wid, hei;
	int head, tail;
	int length;
	int paused, over;
	long long int lastTime;
    
    
	void newFood();
	void update();
	void cleanup();
	int getNewPos(int pos, int dir) const;
	int validPosition(int pos) const;
public:
	Snake();
	~Snake();
	void Create();
	void Play(int key);
	void Pause();
	void instructions();
	void Draw() const;
	void Delete();
};

