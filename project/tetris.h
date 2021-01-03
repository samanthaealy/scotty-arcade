//
//  tetris.h
//  Final Project
//
//  Created by Annie Bae on 11/14/18.
//  Copyright © 2018 Annie Bae. All rights reserved.


#pragma once

#include "Home Screen.h"

class Tetris : public Game
{
private:
	int Board[15][13];
	int blocks[7][4];
	bool start,paused, falling;
	int r, c, b, hei, wid, dt;
	int deleted = 0;
	int pos[4][4];
	bool Playing = false;
	int next;
	//move functions;
	void RotateCW();
	void RotateCCW();
	bool validMove(int row, int col);
	bool validMoveRight(int row, int col);
	bool validMoveLeft(int row, int col);
	//draw functions;
	void DrawStart();
	void DrawGameOver();
	void DrawBoard();
	void DrawNext();
	void DrawPaused();
	//game state
	void CreateBlock();
	bool Gameover(int row, int col);
	void Pause();
	void Update();
	bool rowFull(int r, int col);
	int deleteRows();

public:
	
	Tetris();
	~Tetris();
	void CleanUp();
	void Delete();
	void Draw();
	void Create();
	void Play(int key);

};
