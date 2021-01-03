//
//  Home Screen.cpp
//  Final Project
//
//  Created by Shankar Srinivasan on 11/14/18.
//  Copyright © 2018 Shankar Srinivasan. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"
#include "yspng.h"
#include "SpaceInvaders.h"
#include "brickbreaker.h"
#include "tetris.h"
#include "frogger.h"
#include "Snake.h"

class Arcade
{
public:
    Phoenix P; //1
    brickbreaker Brick; //2
    Tetris T; //3
    Frogger F; //4
    Snake S; //5
    Arcade();
    ~Arcade();
    void Create();
    void Delete();
    void Draw (YsRawPngDecoder *pngPtr);
    int game;
};

Arcade A;

Arcade::Arcade()
{
    game = 0;
}

Arcade::~Arcade()
{
    game = 0;
}

void Arcade::Draw(YsRawPngDecoder *pngPtr)
{

    int winWid,winHei;
    FsGetWindowSize(winWid,winHei);
    glRasterPos2d(0.0,(double)(winHei-1));
    glDrawPixels(pngPtr->wid,pngPtr->hei,GL_RGBA,GL_UNSIGNED_BYTE,pngPtr->rgba);
}

void Render(void *incoming)
{
    YsRawPngDecoder *pngPtr=(YsRawPngDecoder *)incoming;
    
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    switch (A.game)
    {
        case 0:
        {
            A.Draw(pngPtr);
            break;
        }
        case 1:
        {
            A.P.Draw();
            break;
        }
        case 2:
        {
            A.Brick.Draw();
            A.Brick.DrawBlocks();
            break;
        }
        case 3:
        {
            A.T.Draw();
            break;
        }
        case 4:
        {
            A.F.Draw();
            break;
        }
        case 5:
        {
            A.S.Draw();
            break;
        }
            default:
            break;
    }
            
    
    FsSwapBuffers();
    
}

int main(void)
{
    FsChangeToProgramDir();
    YsRawPngDecoder png;
    if(YSOK==png.Decode("homescreen.png"))
    {
        printf("Read Width=%d Height=%d\n",png.wid,png.hei);
        png.Flip();
    }
    else
    {
        printf("Read Error!\n");
        return 0;
    }
    
    FsOpenWindow(16,16,800,800,1);
    FsRegisterOnPaintCallBack(Render,&png);
    
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        
        int lb,mb,rb,mx,my;
        auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
        
        if(FSKEY_ESC==key)
        {
            if (A.game==0)
            {
                break;
            }
            A.game=0;
            A.P.Delete();
            A.Brick.Delete();
            A.T.Delete();
            A.F.Delete();
            A.S.Delete();
        }

		switch (A.game) {
            case 0:
                {
                        if(mx>46 && mx<264 && my>244 && my<463 && lb==1)
                        {
                            A.game = 4;
                            A.F.Create();
                        }
                        if(mx>46 && mx<264 && my>489 && my<707 && lb==1)
                        {
                            A.game = 5;
                            A.S.Create();
                        }
                        if(mx>291 && mx<509 && my>244 && my<463 && lb==1)
                        {
                            A.game = 3;
                            A.T.Create();
                        }
                        if(mx>291 && mx<509 && my>489 && my<707 && lb==1)
                        {
                            A.game = 2;
                            A.Brick.Create();
                        }
                        if(mx>536 && mx<754 && my>244 && my<707 && lb==1)
                        {
                            A.game = 1;
                            A.P.Create();
                        }
                    break;
                }
                
            case 1:
            {
                A.P.Play(key);
                break;
            }
            case 2:
            {
                A.Brick.Play(key);
                break;
            }
            case 3:
            {
                A.T.Play(key);
                break;
            }
            case 4:
            {
                A.F.Play(key);
                break;
            }
            case 5:
            {
                A.S.Play(key);
                break;
            }
                
                
            default:
                break;
        }
        
        FsPushOnPaintEvent();
        FsSleep(25);
    }
}
