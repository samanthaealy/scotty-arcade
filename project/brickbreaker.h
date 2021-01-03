//
//  brickbreaker.hpp
//  BrickBreaker
//
//  Created by Jonathan Dyer on 11/23/18.
//  Copyright © 2018 Jonathan Dyer. All rights reserved.
//

#pragma once

class brickbreaker
{
public:
    //brickboard 
    int brick[4][8], brick_row = 4, brick_col = 8;
    
    //
    void Create();
    void Delete(); 
    
    //brick size
    int bw = 100, bh = 40;
    
    //cannon placement
    int cx = 360, cy = 600, cw = 80, ch = 20;
    
    //cannon size
    int xc, yc, vx, vy, prevx, prevy;
    
    //shooter angle
    int theta = 90, total = 0;
    
    int barrier = 200;
    
    //initializing amount of rectangles that you can't see
    int disappeared = 0;
    
    //variables that toggle Instructions and Pause
    int showInstructions = false;
    int showPause = false;
    int moving = false;
    
    //initializing cannon state
    int cstate = 0;
    
    //intializing number of lives
    int cannons = 3;
    
    double dt = 0.07;
    const double YsPi = 3.1415927;
    void CleanUp();
    void DrawLine(double x1, double y1, double theta);
    void DrawMenu();
    void DrawRectangle(int x, int y, int w, int h, int r, int g, int b);
    void DrawStart();
    void InitializeBoard();
    void Play(int key);
    void DrawBlocks(); 
    void Draw();
    bool checkforcollision(int bx, int by, int tx, int ty, int tw, int th);
    void Pause();
    void Instructions();
    void GameWon();
    void GameOver();
    void DrawCircle(int x, int y, int rad, int r, int g, int b);
    ~brickbreaker();
    brickbreaker();
};
