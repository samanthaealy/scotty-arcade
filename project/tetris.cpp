//
//  tetris.cpp
//  Final Project
//
//  Created by Annie Bae on 11/14/18.
//  Copyright © 2018 Annie Bae. All rights reserved.




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tetris.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"

Tetris::Tetris()
{
	r = 0;
	c = 0;
	b = 0;
}
Tetris::~Tetris()
{
	CleanUp();
}

void Tetris::CleanUp()
{
	r = 0;
	c = 5;
	b = 0;
	hei = 0;
	wid = 0;
	dt = 0;
}
void Tetris::Delete()
{
	CleanUp();
	Create();
    start = false;
    falling = false;
    paused = false;
    Playing = false;
	deleted = 0;
}
void Tetris::CreateBlock()
{
	CleanUp();
	srand(time(NULL));
	c = 5;
	r = 0;
	//b = rand() % 6+1;
	b = next;
	next = rand() % 6 + 1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pos[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int r = (blocks[b][i])/2;
		int c = (blocks[b][i] % 2);
		pos[r][c] = 1;
		if (i == 3)
		{
			wid = c + 1;
			hei = r + 1;
		}
	}
}

void Tetris::DrawNext()
{
	int x = 600;
	int y = 40;
	int buffer = 20;
	int bufferx = x+ buffer;
	int buffery = y + buffer;
	int wid = 30;


	for (int i = 0; i < 4; i++)
	{
		int draw = blocks[next][i];
		int xpos = draw % 2;
		int ypos = draw / 2;
		int blue = 255 * (next % 2);
		int red = 255 * ((next / 2) % 2);
		int green = 255 * ((next / 4) % 2);
		
		if (next != 6)
		{
			ypos += 1;
		}

		int x1 = bufferx + wid*(xpos + 1);
		int y1 = buffery + wid*(ypos);

		int x2 = x1 + wid;
		int y2 = y1 + wid;

		glBegin(GL_QUADS);
		glColor3ub(red, green, blue);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
		glEnd();
	}


	// draw lattice;
	int row = 4;
	int col = 4;

	for (int i = 0; i < col + 1; i++)
	{
		glBegin(GL_LINES);
		glColor3ub(0, 0, 0);
		int x1 = wid * (i) + bufferx;
		glVertex2i(x1, buffery);
		glVertex2i(x1, buffery+120);
		glEnd();
		glFlush();
	}

	for (int j = 0; j < row + 1; j++)
	{
		glBegin(GL_LINES);
		glColor3ub(0, 0, 0);
		int y1 = wid * (j ) + buffery;
		glVertex2i(bufferx, y1);
		glVertex2i(bufferx + 120 , y1);
		glEnd();
		glFlush();
	}

}


void Tetris::Draw()
{
	int w = 40;
	int blue = 255 * (b % 2);
	int red = 255 * ((b / 2) % 2);
	int green = 255 * ((b / 4) % 2);
	int x1, y1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (pos[i][j] == 1)
			{
				int brow = r + i;
				int bcol = c + j;
				int x = (bcol + 1)*w;
				int y = (brow + 1)*w;

				glBegin(GL_QUADS);
				glColor3ub(red, green, blue);
				glVertex2i(x, y);
				glVertex2i(x+w, y);
				glVertex2i(x+w, y+w);
				glVertex2i(x, y+w);
				glEnd();
			}
		}
	}
	DrawBoard();
	DrawNext();
	if (Gameover(15, 13))
	{
		DrawGameOver();
	}
	if (Playing == false)
	{
		DrawStart();
	}
	if (paused)
	{
		DrawPaused();
	}
}

void Tetris::RotateCW()
{
	int centerR = r + hei / 2;
	int centerC = c + wid / 2;
	int old[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			old[i][j] = pos[i][j];
			pos[i][j] = 0;
		}
	}
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (old[row][col] == 1)
			{
				pos[col][hei - 1 - row] = 1;
			}
		}
	}
	int temp = hei;
	hei = wid;
	wid = temp;
	r = centerR - hei / 2;
	c = centerC - wid / 2;

}

void Tetris::RotateCCW()
{
	int centerR = r + hei / 2;
	int centerC = c + wid / 2;
	int old[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			old[i][j] = pos[i][j];
			pos[i][j] = 0;
		}
	}
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (old[row][col] == 1)
			{
				pos[wid-1-col][row] = 1;
			}
		}
	}
	int temp = hei;
	hei = wid;
	wid = temp;
	r = centerR - hei / 2;
	c = centerC - wid / 2;
}

bool Tetris:: validMove(int row, int col)
{
	if (r + hei == row)
	{
		return false;
	}
	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			if (pos[i][j] == 1 && Board[r + i + 1][c + j] != -1)
			{
				return false;
			}
		}
	}
	return true;
}

bool Tetris::validMoveRight(int row, int col)
{
	if (c + wid == col)
	{
		return false;
	}
	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			if (pos[i][j] == 1 && Board[r + i][c + j + 1] != -1)
			{
				return false;
			}
		}
	}
	return true;

}

bool Tetris::validMoveLeft(int row, int col)
{
	if (c-1 < 0)
	{
		return false;
	}
	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			if (pos[i][j] == 1 && Board[r + i][c + j - 1] != -1)
			{
				return false;
			}
		}
	}
	return true;

}

void Tetris::Update()
{
	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			if (pos[i][j] == 1)
			{
				Board[i + r][j + c] = b;
			}
		}
	}
}


void Tetris::DrawBoard()
{
	int row = 15;
	int col = 13;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int b = Board[i][j];
			if (b != -1)
			{
				int dx = 40;
				int blue = 255 * (b % 2);
				int red = 255 * ((b / 2) % 2);
				int green = 255 * ((b / 4) % 2);
				int y1 = (i + 1)*dx;
				int x1 = (j + 1)*dx;
				int x2 = x1 + dx;
				int y2 = y1 + dx;
			
				glBegin(GL_QUADS);
				glColor3ub(red, green, blue);
				glVertex2i(x1, y1);
				glVertex2i(x2, y1);
				glVertex2i(x2, y2);
				glVertex2i(x1, y2);
				glEnd();
			}
		}
	}
	//draw lattice

	for (int i = 0; i < col + 1; i++)
	{
		glBegin(GL_LINES);
		glColor3ub(0, 0, 0);
		int x1 = 40 * (i + 1);
		glVertex2i(x1, 40);
		glVertex2i(x1, 640);
		glEnd();
		glFlush();
	}

	for (int j = 0; j < row + 1; j++)
	{
		glBegin(GL_LINES);
		glColor3ub(0, 0, 0);
		int y1 = 40 * (j + 1);
		glVertex2i(40, y1);
		glVertex2i(560, y1);
		glEnd();
		glFlush();
	}
	char str[256];
	//Draw next box
	glColor3ub(200, 20, 140);
	glRasterPos2i(600,40);
	sprintf(str, "Next");
	YsGlDrawFontBitmap16x20(str);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2i(600, 40);
	glVertex2i(760, 40);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2i(760, 40);
	glVertex2i(760, 200);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2i(760, 200);
	glVertex2i(600, 200);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2i(600, 200);
	glVertex2i(600, 40);
	glEnd();
	glFlush();
	glColor3ub(200, 20, 140);
	glRasterPos2i(20, 750);
	sprintf(str, "Lines : %d",deleted);
	YsGlDrawFontBitmap16x20(str);
	
}

void Tetris:: Create()
{
	next = rand() % 6 + 1;
	int row = 15;
	int col = 13;
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			Board[r][c] = -1;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0:
			blocks[i][0] = 1;
			blocks[i][1] = 2;
			blocks[i][2] = 3;
			blocks[i][3] = 4;
			break;
		case 1:
			blocks[i][0] = 0;
			blocks[i][1] = 1;
			blocks[i][2] = 2;
			blocks[i][3] = 3;
			break;
		case 2:
			blocks[i][0] = 0;
			blocks[i][1] = 2;
			blocks[i][2] = 3;
			blocks[i][3] = 5;
			break;
		case 3:
			blocks[i][0] = 0;
			blocks[i][1] = 2;
			blocks[i][2] = 4;
			blocks[i][3] = 5;
			break;
		case 4:
			blocks[i][0] = 1;
			blocks[i][1] = 3;
			blocks[i][2] = 4;
			blocks[i][3] = 5;
			break;
		case 5:
			blocks[i][0] = 1;
			blocks[i][1] = 2;
			blocks[i][2] = 3;
			blocks[i][3] = 5;
			break;
		case 6:
			blocks[i][0] = 0;
			blocks[i][1] = 2;
			blocks[i][2] = 4;
			blocks[i][3] = 6;
			break;
		}
	}

	start = false;
	falling = false;
	paused = false;
}
void Tetris::Pause()
{
	paused = !paused;
}
bool Tetris::rowFull(int r, int col)
{
	for (int c = 0; c < col; c++)
	{
		if (Board[r][c] == -1)
		{
			return false;
		}
	}
	return true;
}
int Tetris::deleteRows()
{
	const int row = 15;
	const int col = 13;
	bool change = false;
	int nr = row-1;
	int deleted = 0;
	int newBoard[row][col];
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			newBoard[i][j] = -1;
		}
	}
	for (int i = row-1; i >= 0; i--)
	{
		if (!rowFull(i, col))
		{
			for (int j = 0; j < col; j++)
			{
				change = true;
				newBoard[nr][j] = Board[i][j];
			}
			nr -= 1;
			deleted += 1;
		}
	}
	if (change)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				Board[i][j] = newBoard[i][j];
			}
		}
	}
	return deleted;
}

void Tetris::DrawStart()
{
	glBegin(GL_QUADS);
	glColor3ub(255, 185, 235);
	glVertex2i(120, 200);
	glVertex2i(480, 200);
	glVertex2i(480, 400);
	glVertex2i(120, 400);
	glEnd();
	glFlush();

	glColor3ub(200, 20, 140);
	glRasterPos2i(150,240);
	char str[256];
	sprintf(str, "S to start");
	YsGlDrawFontBitmap16x20(str);

	glRasterPos2i(150, 280);
	sprintf(str, "P to pause");
	YsGlDrawFontBitmap16x20(str);

	glRasterPos2i(146, 320);
	sprintf(str, "left & right to move");
	YsGlDrawFontBitmap16x20(str);

	glRasterPos2i(150, 360);
	sprintf(str, "up & down to rotate");
	YsGlDrawFontBitmap16x20(str);
}


void Tetris::DrawPaused()
{
	glBegin(GL_QUADS);
	glColor3ub(255, 185, 235);
	glVertex2i(120, 200);
	glVertex2i(480, 200);
	glVertex2i(480, 400);
	glVertex2i(120, 400);
	glEnd();
	glFlush();

	glColor3ub(200, 20, 140);
	glRasterPos2i(150, 240);
	char str[256];

	glRasterPos2i(150, 265);
	sprintf(str, "P to play");
	YsGlDrawFontBitmap16x20(str);

	glRasterPos2i(146, 305);
	sprintf(str, "left & right to move");
	YsGlDrawFontBitmap16x20(str);

	glRasterPos2i(150, 345);
	sprintf(str, "up & down to rotate");
	YsGlDrawFontBitmap16x20(str);
}





void Tetris::DrawGameOver()
{
	glBegin(GL_QUADS);
	glColor3ub(255, 185, 235);
	glVertex2i(150, 200);
	glVertex2i(450, 200);
	glVertex2i(450, 400);
	glVertex2i(150, 400);
	glEnd();
	glFlush();

	glColor3ub(200, 20, 140);
	glRasterPos2i(220, 280);
	char str[256];
	sprintf(str, "Game Over!");
	YsGlDrawFontBitmap16x20(str);

	glColor3ub(200, 20, 140);
	glRasterPos2i(200, 330);
	sprintf(str, "R to restart");
	YsGlDrawFontBitmap16x20(str);
}

//game starts

bool Tetris::Gameover(int row, int col)
{
	for (int c = 0; c < col; c++)
	{
		if (Board[0][c] != -1)
		{
			return true;
		}
	}
	return false;
}

void Tetris::Play(int key)
{
	int row = 15;
	int col = 13;
	bool gameover = Gameover(row, col);
	dt += FsPassedTime();
	if (gameover == true)
	{
		if (key == FSKEY_R)
		{
			Create();
			deleted = 0;
			gameover = false;
			start = true;
			falling = false;
			paused = false;
			dt = 0;
		}
	}

	if (!gameover)
	{
		dt += FsPassedTime();
		if (key == FSKEY_S)
		{
			if (!start)
			{
				Create();
				start = true;
				Playing = true;
			}
		}

		if (start == true)
		{
			if (!falling && !paused)
			{
				CreateBlock();
				falling = true;
				dt = 0;
			}
			if (key == FSKEY_P)
			{
				Pause();
			}

			if (falling && !paused)
			{

				int dx = 40;
				int right, bottom;
				right = c + wid;
				bottom = r + hei;

				if (key == FSKEY_RIGHT)
				{
					if (validMoveRight(row, col))
					{
						c += 1;
					}
				}
				if (key == FSKEY_LEFT)
				{
					if (validMoveLeft(row, col))
					{
						c -= 1;
					}
				}
				if (key == FSKEY_UP)
				{
					if (b != 1)
					{
						if (wid < hei)
						{
							if (c + hei < col + 1 && c + wid - hei > -1)
							{
								RotateCW();
							}
						}
						else
						{
							if (r + wid < row + 1 && r + hei - wid > -1)
							{
								RotateCW();
							}
						}
					}
				}
				if (key == FSKEY_DOWN)
				{
					if (b != 1)
					{
						if (wid < hei)
						{
							if (c + hei < col + 1 && c + wid - hei > -1)
							{
								RotateCCW();
							}
						}
						else
						{
							if (r + wid < row + 1 && r + hei - wid >-1)
							{
								RotateCCW();
							}
						}
					}
				}

				
				if (dt / 200 > 0)
				{
					if (validMove(row, col))
					{
						r += 1;

					}
					else
					{
						Update();
						int d = deleteRows();
						deleted += row - d;

						FsPushOnPaintEvent();
						falling = false;
					}
					dt = 0;

				}
			}

		}
	}
}




