//
//  frogger.cpp
//  frogger
//
//  Created by Samantha Ealy on 11/21/18.
//  Copyright © 2018 CMU. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "frogger.h"
using namespace std;

const double YS_PI = 3.1415927;
const double cellSize = 50.0;

#define SAFETY 0
#define CAR 1
#define BUS 2
#define LOG 3
#define HOME 4

Rectangle::Rectangle()
{
    x = 0.0;
    y = 0.0;
    w = 0.0;
    h = 0.0;
}

Rectangle::~Rectangle()
{
    Delete();
}

void Rectangle::Delete(void)
{
    x = 0.0;
    y = 0.0;
    w = 0.0;
    h = 0.0;
}

void Rectangle::SetColor(int red, int green, int blue)
{
    r = red;
    g = green;
    b = blue;
}

bool Rectangle::Intersect(Rectangle obs)
{
    double left = x;
    double right = x + w;
    double top = y;
    double bottom = y + h;
    
    double obs_left = obs.x;
    double obs_right = obs.x + obs.w;
    double obs_top = obs.y;
    double obs_bottom = obs.y + obs.h;
    
    return !(left >= obs_right || right <= obs_left ||
             top >= obs_bottom || bottom <= obs_top);
}


void Player::Create(void)
{
    x = 400.0 - cellSize/2.0;
    y = 700.0 - cellSize;
    w = cellSize;
    h = cellSize;
}

void Player::Move(double dx, double dy)
{
    // only move if you are in bounds (14 rows, 15 cols)
    if (-25.0 <= x + dx * w && x + w + dx * w <= 825.0
         && 0 <= y + dy * h && y + h + dy * h <= 700.0)
    {
        x += dx * w;
        y += dy * h;
    }
}

void Player::Draw(void) const
{
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2d(x, y);
    glVertex2d(x + w, y);
    glVertex2d(x + w, y + h);
    glVertex2d(x, y + h);
    glEnd();
}


int Player::GetLane(void)
{
    return y/cellSize;
}

void Homes::Create(int i, double spc)
{
    x = 1.5 * cellSize * i + spc + 1.25 * cellSize;
    y = cellSize;
    w = 1.5 * cellSize;
    h = cellSize;
    state = 0;
    SetColor(0, 102, 0);
}

void Homes::Draw() const
{
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2d(x, y);
    glVertex2d(x + w, y);
    glVertex2d(x + w, y + h);
    glVertex2d(x, y + h);
    glEnd();
}

void Obstacles::Create(int row, int col, string o, double vel, double spc)
{
    if (vel <= 0)
    {
        x = cellSize * o.length() * col + spc;
        if(vel == 0)
        {
            x -= 1.25 * cellSize;
        }
    }
    else
    {
        x = -950 + cellSize * col + spc;
    }
    y = cellSize * row;
    w = cellSize * o.length();
    if (vel == 0)
    {
        w -= cellSize/2;
    }
    h = cellSize;
    AssignColor(o);
}

void Obstacles::Move(double vel)
{
    x += vel;
    if (vel > 0 && x > 800.0)
    {
        x = -35 * cellSize + 800.0;
    }
    else if (vel < 0 && x < -w)
    {
        x = 35 * cellSize;
    }
}

void Obstacles::AssignColor(string type)
{
    switch(type[0])
    {
        case 'l':
            r = 102;
            g = 51;
            b = 0;
            break;
        case 'b':
            r = 0;
            g = 105;
            b = 0;
            break;
        case 'c':
            r = 255;
            g = 0;
            b = 0;
            break;
        case 'w':
            r = 76;
            g = 153;
            b = 0;
            break;
    }
    SetColor(r, g, b);
}

void Obstacles::Draw(void) const
{
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2d(x, y);
    glVertex2d(x + w, y);
    glVertex2d(x + w, y + h);
    glVertex2d(x, y + h);
    glEnd();
}

void Lanes::Create(int i, int type)
{
    x = 0;
    y = cellSize * i;
    w = 800.0;
    h = cellSize;
    obsVec = {};
    nObs = 0;
    
    switch(type)
    {
        case SAFETY:
            CreateSafetyLane();
            break;
        case HOME:
            CreateHomeLane();
            break;
        case LOG:
            CreateLogLane();
            if (i % 2 != 0)
            {
                vel = -vel;
            }
            break;
        case CAR:
            CreateCarLane();
            break;
        case BUS:
            CreateBusLane();
            vel = -vel;
            break;
    }
    if (laneType != SAFETY)
    {
        nObs = (int)obsVec.size()/2;
        obs = new Obstacles[nObs];
        double spacing = 0;
        int tally = 0;
        for (unsigned j = 0; j < obsVec.size(); j++)
        {
            string str = obsVec.at(j);
            if(str.find(".") != string::npos)
            {
                spacing += cellSize * str.length();
            }
            else
            {
                obs[j/2].Create(i, j/2, str, vel, spacing);
            }
            for (int k = 0; k < str.length(); k++)
            {
                tally++;
            }
        }
        printf("%d %d\n", laneType, tally);
    }
}



void Lanes::Draw(void) const
{
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2d(x, y);
    glVertex2d(x + w, y);
    glVertex2d(x + w, y + h);
    glVertex2d(x, y + h);
    glEnd();
    if (laneType != SAFETY)
    {
        for (int i = 0; i < nObs; i++)
        {
            obs[i].Draw();
        }
    }
}

void Lanes::Delete(void)
{
    Rectangle::Delete();
    laneType = 0;
    vel = 0.0;
    r = 0;
    g = 0;
    b = 0;
    obs = nullptr;
}

void Lanes::Move(void)
{
    for (int i = 0; i < nObs; i++)
    {
        obs[i].Move(vel);
    }
}

void Lanes::CreateSafetyLane(void)
{
    laneType = SAFETY;
    vel = 0.0;
    SetColor(76, 153, 0);
    obsVec.push_back("...................................");
}

void Lanes::CreateHomeLane(void)
{
    laneType = HOME;
    vel = 0.0;
    SetColor(51, 153, 255);
    string wallObs[6] = {"ww", "ww", "ww", "ww", "ww", "ww"};
    string wallSpc[7] = {".", ".", ".", ".", ".", ".", "."};
    SetCourse(6, wallObs, wallSpc);
}

void Lanes::CreateLogLane(void)
{
    laneType = LOG;
    vel = double(rand() % 8) + 4.0;
    SetColor(51, 153, 255);
    string logObs[3] = {"ll", "llll", "lllll"};
    string logSpc[4] = {"....", ".....", "......", "........."};
    SetCourse(3, logObs, logSpc);
}

void Lanes::CreateCarLane(void)
{
    laneType = CAR;
    vel = double(rand() % 8) + 4.0;
    SetColor(128, 128, 128);
    string carObs[3] = {"cc", "cc", "cc"};
    string carSpc[4] = {"....", ".......", ".........", "........."};
    SetCourse(3, carObs, carSpc);
}

void Lanes::CreateBusLane(void)
{
    laneType = BUS;
    vel = double(rand() % 8) + 4.0;
    SetColor(128, 128, 128);
    string busObs[3] = {"bbb", "bbb", "bbb"};
    string busSpc[4] = {"...","......", "........", "........."};
    SetCourse(3, busObs, busSpc);
}


void Lanes::SetCourse(int numObs, string obs[], string spc[])
{
    // Shuffle arrays
    for (int i = 0; i < numObs; i++)
    {
        int n = rand() % numObs;
        SwapString(obs[i], obs[n]);
    }
    for (int i = 0; i < numObs + 1; i++)
    {
        int n = rand() % (numObs + 1);
        SwapString(spc[i], spc[n]);
    }
    // Merge arrays
    for (int i = 0; i < 2 * numObs + 1; i++)
    {
        int j = i/2;
        string str;
        if (i % 2 == 0)
        {
            str = spc[j];
        }
        else
        {
            str = obs[j];
        }
        obsVec.push_back(str);
    }
}

void Lanes::SwapString(string &a, string &b)
{
    string c;
    c = b;
    b = a;
    a = c;
}

Frogger::Frogger()
{
    paused = false;
    gameOver = false;
    levelUp = false;
    text = true;
    nLanes = 14;
    nLives = 5;
    nHomes = 5;
    score = 0;
    level = 0;
    timer = 0;
    t0 = time(NULL);
}

Frogger::~Frogger()
{
    Delete();
}

void Frogger::Delete(void)
{
    frog.Delete();
    for (int i = 0; i < nLanes; i++)
    {
        lane[i].Delete();
    }
    for (int i = 0; i < nHomes; i++)
    {
        home[i].Delete();
    }
    paused = 0;
    gameOver = 0;
    nLanes = 0;
    nLives = 0;
    nHomes = 0;
    score = 0;
    level = 0;
    timer = 0;
    text = 0;
}

void Frogger::Draw(void) const
{
    DrawBackground();
    for (int i = 0; i < nLanes; i++)
    {
        lane[i].Draw();
    }
    for (int i = 0; i < 5; i++)
    {
        if (home[i].state != 0)
        {
            home[i].Draw();
        }
    }
    frog.Draw();
    DrawOverlay();
    DrawTitle();
}

void Frogger::Create(void)
{
    paused = false;
    gameOver = false;
    nLanes = 14;
    nLives = 5;
    nHomes = 5;
    timer = time(NULL);
    frog.Create();
    int type = 0;
    for(int i = 0; i < nLanes; i ++)
    {
        switch(i)
        {
            case 0:
            case 7:
            case 13:
                type = SAFETY;
                break;
            case 1:
                type = HOME;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                type = LOG;
                break;
            case 8:
            case 10:
            case 12:
                type = CAR;
                break;
            case 9:
            case 11:
                type = BUS;
                break;
        }
        lane[i].Create(i, type);
    }
    for (int i = 0; i < nHomes; i++)
    {
        home[i].Create(i, 1.5 * cellSize* i);
    }
    if (level == 0)
    {
        t0 = time(NULL);
        score = 0;
    }
}

void Frogger::Play(int key)
{

    if(FSKEY_SPACE == key)
    {
        Pause();
    }
    
    if (FSKEY_R == key && gameOver == true)
    {
        gameOver = false;
        Create();
    }
    
    if (!paused && !gameOver)
    {
        if (FSKEY_UP == key)
        {
            frog.Move(0, -1);
            score += 10;
        }
        if (FSKEY_LEFT == key)
        {
            frog.Move(-1, 0);
        }
        if (FSKEY_DOWN == key)
        {
            frog.Move(0, 1);
            score -= 10;
        }
        if (FSKEY_RIGHT == key)
        {
            frog.Move(1, 0);
        }
        for (int i = 0; i < nLanes; i++)
        {
            lane[i].Move();
        }

        int nLogsIntersected = 0;
        for (int i = 0; i < lane[frog.GetLane()].nObs; i++)
        {
            // Only check obstacles in frog's current lane
            if(frog.Intersect(lane[frog.GetLane()].obs[i]) &&
               (lane[frog.GetLane()].laneType == CAR ||
                lane[frog.GetLane()].laneType == BUS ||
                lane[frog.GetLane()].laneType == HOME))
            {
                frog.Create();
                nLives--;
                timer = time(NULL);
            }
            else if (lane[frog.GetLane()].laneType == LOG)
            {
                if (frog.Intersect(lane[frog.GetLane()].obs[i]))
                {
                    nLogsIntersected++;
                }
            }
        }
        if (lane[frog.GetLane()].laneType == LOG)
        {
            if (nLogsIntersected > 0)
            {
                frog.x += lane[frog.GetLane()].vel;
            }
            else
            {
                frog.Create();
                nLives--;
                timer = time(NULL);
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if(frog.Intersect(home[i]) && home[i].state == 0)
            {
                score += 50;
                nHomes--;
                home[i].state = 1;
                // Award 10 points for ever 1/2 second of time unused
                unsigned long sec = time(NULL) - timer;
                score += 2 * (45 - sec) * 10;
                timer = time(NULL);
                frog.Create();
            }
        }
        
        if (nHomes == 0)
        {
            score += 1000;
            level++;
            levelUp = true;
            t0 = time(NULL);
            Create();
        }
    
        // Reset frog position if the frog goes off the screen
        if (frog.x + frog.w < 0 || frog.x > 800.0)
        {
            nLives--;
            frog.Create();
            timer = time(NULL);
        }
        
    }
    
    if (nLives <= 0)
    {
        gameOver = true;
    }
    
    if (time(NULL) - timer >= 45 && !gameOver)
    {
        nLives--;
        timer = time(NULL);
        frog.Create();
    }
    
    if (time(NULL) - t0 < 5 && level == 0)
    {
        text = true;
    }
    else if (time(NULL) - t0 > 1 && levelUp)
    {
        levelUp = false;
    }
    else
    {
        text = false;
    }
    
}

void Frogger::Pause(void)
{
    if(!paused)
    {
        paused = true;
    }
    else
    {
        paused = false;
    }
}

void Frogger::DrawBackground(void) const
{
    glBegin(GL_QUADS);
    
    glColor3ub(0, 0, 0);
    
    glVertex2d(0, 0);
    glVertex2d(800, 0);
    glVertex2d(800, 800);
    glVertex2d(0, 800);
    
    glColor3ub(255, 255, 255);
    
    glVertex2d(25, 0);
    glVertex2d(775, 0);
    glVertex2d(775, 700);
    glVertex2d(25, 700);
    
    glEnd();
}

void Frogger::DrawOverlay(void) const
{
    glColor3ub(255, 255, 255);
    glRasterPos2d(20.0, 730.0);
    YsGlDrawFontBitmap16x20("SCORE");
    
    const string str = to_string(score);
    char const *ch = str.c_str();
    glRasterPos2d(115.0, 730.0);
    YsGlDrawFontBitmap16x20(ch);
    
    glRasterPos2d(720.0, 730.0);
    YsGlDrawFontBitmap16x20("TIME");
    
    // DRAW TIME BAR
    if (!gameOver)
    {
        glBegin(GL_QUADS);
        glVertex2d(565 + (double(time(NULL))-timer)*3.0, 710.0);
        glVertex2d(700.0, 710.0);
        glVertex2d(700.0, 730.0);
        glVertex2d(565 + (double(time(NULL))-timer)*3.0, 730.0);
        glEnd();
    }
    
    glRasterPos2d(15.0, 35.0);
    YsGlDrawFontBitmap16x20("LIVES");
    for (int i = 0; i < nLives; i++)
    {
        glBegin(GL_TRIANGLE_FAN);
        for(int j = 0; j < 360; j += 30)
        {
            glColor3ub(0, 102, 0);
            double a = double(j)*YS_PI/180.0;
            double s = sin(a);
            double c = cos(a);
            
            double xx = double(40.0*i + 125.0) + c*double(15.0);
            double yy = double(25.0) + s*double(15.0);
            glVertex2i(int(xx), int(yy));
        }
        glEnd();
    }
}

void Frogger::DrawTitle(void) const
{
    if (paused)
    {
        glColor3ub(255, 255, 255);
        glRasterPos2d(340, 150);
        YsGlDrawFontBitmap20x28("Paused");
        DrawInstructions();
    }
    if (gameOver)
    {
        glColor3ub(0, 0, 0);
        glRasterPos2d(320, 350);
        YsGlDrawFontBitmap20x28("GAME OVER");
        glColor3ub(255, 0, 0);
        glRasterPos2d(321, 351);
        YsGlDrawFontBitmap20x28("GAME OVER");
        DrawGameOver();
    }
    if (text)
    {
        DrawInstructions();
    }
    if (levelUp)
    {
        glColor3ub(255, 255, 255);
        glRasterPos2d(320, 350);
        YsGlDrawFontBitmap20x28("Level Up!");
    }
}

void Frogger::DrawGameOver(void) const
{
    glColor3ub(0, 0, 0);
    glRasterPos2d(200.0,380.0);
    YsGlDrawFontBitmap16x20("Use 'R' to Restart");
    glRasterPos2d(200.0,400.0);
    YsGlDrawFontBitmap16x20("Use ESC to Return to Arcade");
}

void Frogger::DrawInstructions(void) const
{
    glColor3ub(255, 255, 255);
    glRasterPos2d(200.0,300.0);
    YsGlDrawFontBitmap16x20("Hit SPACE pause/unpause");
    glRasterPos2d(200.0,350.0);
    YsGlDrawFontBitmap16x20("Use the Arrows to Move");
    glRasterPos2d(200.0,400.0);
    YsGlDrawFontBitmap16x20("Use ESC to Return to Arcade");
}

