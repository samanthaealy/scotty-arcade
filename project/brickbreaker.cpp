#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "brickbreaker.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <math.h>


brickbreaker::brickbreaker()
{
    cx = 360;
    cy = 600;
    cw = 80;
    ch = 20;
}

brickbreaker::~brickbreaker()
{
    CleanUp();
}

void brickbreaker::Create()
{
    Delete();
//    Instructions(); 
}

void brickbreaker::Delete()
{
    CleanUp();
}

void brickbreaker::CleanUp()
{
    cannons = 3;
    for (int i = 0; i < brick_row; i++)
    {
        for (int j = 0; j < brick_col; j++)
        {
            brick[i][j] = 1;
        }
    }
    cstate = 0;
    disappeared = 0;
}

void brickbreaker::DrawMenu()
{
    glBegin(GL_LINES);
    glColor3b(1.0f, 0, 0);
    glVertex2i(0, 660);
    glVertex2i(800,660);
    glEnd();
    glFlush();
    
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2d(0, 660);
    glVertex2d(800, 660);
    glVertex2d(800, 800);
    glVertex2d(0, 800);
    glEnd();
    glFlush();
    
    glColor3f(0, 1.0f, 0);
    glRasterPos2d(10, 690);
    YsGlDrawFontBitmap16x20("Lives:");
    
    glColor3f(0, 1.0f, 0);
    glRasterPos2d(180, 680);
    YsGlDrawFontBitmap8x12("Press I for Instructions, Press ESC to Return to the Main Menu");
    
    glColor3f(0, 1.0f, 0);
    glRasterPos2d(180, 700);
    YsGlDrawFontBitmap8x12("Press P to Pause and Press A to Restart");
    
    for (int i = 0; i < cannons; i++)
    {
        int x = 100 + 20 * (i + 1);
        int y = 680;
        DrawCircle(x, y, 5, 255, 0, 0);
    }
}

void brickbreaker::DrawLine(double x1, double y1, double theta)
{
    //draw the line for the cannon
    glBegin(GL_LINES);
    glColor3b(1.0, 1.0, 1.0);
    x1 += 40.0;
    glVertex2d(x1, y1);
    double x2 = x1 + 20.0 * cos(theta*YsPi / 180.0);
    double y2 = y1 - 20.0 * sin(theta*YsPi / 180.0);
    glVertex2d(x2, y2);
    glEnd();
    glFlush();
}

void brickbreaker::DrawRectangle(int x, int y, int w, int h, int r, int g, int b)
{
    // draw the obstacles
    glBegin(GL_QUADS);
    glColor3ub(r,g,b);
    
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
    glFlush();
    
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    
    glBegin(GL_LINES);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    
    glBegin(GL_LINES);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    
    glBegin(GL_LINES);
    glVertex2i(x, y + h);
    glVertex2i(x, y);
    
    glEnd();
    glFlush();
}

void brickbreaker::DrawCircle(int x, int y, int rad, int r, int g, int b)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(r,g,b);
    for (int i = 0; i<360; i += 30)
    {
        double a = (double)i*YsPi / 180.0;
        double s = sin(a);
        double c = cos(a);
        
        double xx = (double)x + c * (double)rad;
        double yy = (double)y + s * (double)rad;
        glVertex2d(xx, yy);
    }
    glEnd();
    glFlush();
}

void brickbreaker::InitializeBoard()
{
    for (int i = 0; i < brick_row; i++)
    {
        for (int j = 0; j < brick_col; j++)
        {
            brick[i][j] = 1;
        }
    }
}

bool brickbreaker::checkforcollision(int bx, int by, int tx, int ty, int tw, int th)
{
    //check for collision
    int dx = bx - tx;
    int dy = by - ty;
    
    if (0 <= dx && dx <= tw && 0 <= dy && dy <= th)
    {
        return true;
    }
    return false;
}

void brickbreaker::Draw()
{
    DrawRectangle(cx, cy, cw, ch, 0, 0, 255);
    DrawLine(cx, cy, theta);
    DrawMenu();
    if (cstate == 1)
    {
        DrawCircle(xc, yc, 10,0,0,0);
    }
    if (showPause)
    {
//        showInstructions = false;
        Pause();
    }
    
    if (showInstructions)
    {
//        showPause = false;
        Instructions();
        //                printf("Pressed");
    }
    
    if (disappeared == brick_row * brick_col)
    {
        GameWon();
        
    }
    
    else if(cannons == 0 && 0 == cstate)
    {
        GameOver();
    }
}

void brickbreaker::DrawBlocks()
{
    for (int r = 0; r < brick_row; r++)
    {
        for (int c = 0; c < brick_col; c++)
        {
            //if the break exists
            if (brick[r][c] != 0)
            {
                int bx = bw * c;
                int by = bh * r;
                
                //                    int dx = 255 / brick_col;
                //                    int dy = 255 / brick_row;
                
                //draws each row in the colors of the classic brick breakers
                int r1,g,b;
                
                switch(r)
                {
                    case 0:
                        r1 = 0;
                        g = 255;
                        b = 255;
                        break;
                    case 1:
                        r1 = 0;
                        g = 250;
                        b = 154;
                        break;
                    case 2:
                        r1 = 255;
                        g = 255;
                        b = 0;
                        break;
                    case 3:
                        r1 = 255;
                        g = 140;
                        b = 0;
                        break;
                }
                DrawRectangle(bx, by, bw, bh, r1, g, b);
            }
        }
    }
}

void brickbreaker::Pause()
{
    showInstructions = false;
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(320, 300.0);
    YsGlDrawFontBitmap16x20("Paused");
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(160, 340.0);
    YsGlDrawFontBitmap16x20("(Press P again to Resume Playing)");
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(140, 380.0);
    YsGlDrawFontBitmap16x20("(Press ESC to Return to the Main Menu)");
}

void brickbreaker::Instructions()
{
    showPause = false;
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(110, 220.0);
    YsGlDrawFontBitmap16x20("Use the Left & Right Arrow Keys to Move");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(150, 260.0);
    YsGlDrawFontBitmap16x20("Press E to Aim Left & R to Aim Right");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(250, 300.0);
    YsGlDrawFontBitmap16x20("Press Space to Fire");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(25, 340.0);
    YsGlDrawFontBitmap16x20("You have 3 Lives to Get Rid of All the Blocks");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(320, 400.0);
    YsGlDrawFontBitmap16x20("Good Luck!");
}

void brickbreaker::GameWon()
{
    // user won the game.
    glColor3f(1.0f, 0, 1.0f);
    glRasterPos2d(320, 300.0);
    YsGlDrawFontBitmap16x20("You Won!");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(220, 340.0);
    YsGlDrawFontBitmap16x20("Press A to Play Again");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(140, 380.0);
    YsGlDrawFontBitmap16x20("Press ESC to Return to the Main Menu");
}

void brickbreaker::GameOver()
{
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(320, 300.0);
    YsGlDrawFontBitmap16x20("GAME OVER");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(220, 340.0);
    YsGlDrawFontBitmap16x20("Press A to Play Again");
    
    glColor3f(1.0f, 0, 0);
    glRasterPos2d(140, 380.0);
    YsGlDrawFontBitmap16x20("Press ESC to Return to the Main Menu");
}

void brickbreaker::Play(int key)
{
//    if (FSKEY_ESC == key)
//    {
//        terminate = true;
//    }
    
    if (FSKEY_R == key)
    {
        if (theta > 20)
        {
            theta -= 5;
        }
    }
    
    else if (FSKEY_E == key)
    {
        if (theta < 160)
        {
            theta += 5;
        }
    }
    
    else if (FSKEY_M == key)
    {
        //add code to return to the main menu
    }
    
    else if (FSKEY_LEFT == key)
    {
        if (cx != 0)
        {
            cx -= 20;
        }
    }
    
    else if (FSKEY_A == key)
    {
        CleanUp();
    }
    
    else if (FSKEY_RIGHT == key)
    {
        if (cx != 800-cw)
        {
            cx += 20;
        }
    }
    
    else if (FSKEY_SPACE == key)
    {
        // if there is no cannon
        if (0 == cstate && cannons > 0 && disappeared < brick_row*brick_col && showPause == false && showInstructions == false)
        {
            //fire a cannon
            cannons -= 1;
            cstate = 1;
            
            //set the center of the cannon to be the end of the drawn line
            xc = cx + cw/2 + 20.0 * cos(theta*YsPi / 180.0);
            yc = cy - 20.0 * sin(theta*YsPi / 180.0);
            
            //initialize x and y velocity;
            vx = 200.0*cos(theta*YsPi / 180.0);
            vy = 200.0*sin(theta*YsPi / 180.0);
        }
    }
    
    if (cstate == 1)
    {
        //draw the cannon ball at the start
//        DrawCircle(xc, yc, 10,0,0,0);
        
        //change the position of the cannonball to depending on the velocity
        prevx = xc;
        prevy = yc;
        xc += vx * dt;
        yc -= vy * dt;
        
        // if the cannon ball crosses the control panel, reset the game
        if (yc > 660)
        {
            cstate = 0;
        }
        if (xc >= 800)
        {
            vx = -vx;
        }
        else if (xc <= 0)
        {
            vx = -vx;
        }
        else if (yc <= 0)
        {
            vy = -vy;
        }
    }
    
    if (0 != cstate)
    {
        if (checkforcollision(xc, yc, cx, cy, cw, ch))
        {
            vy = -vy;
            vx += 30;
            if (vy < 0)
            {
                vy = -vy;
            }
        }
    }
    
    // if the cannonball has been fired
    if (0 != cstate)
    {
        int vxprev, vyprev, loopcount = 1;
        moving = true; 
        if (FSKEY_P == key)
        {
//            vxprev = vx;
//            vyprev = vy;
            showPause = !showPause;
//            printf("showPause = %d",showPause);
            moving = !moving;
//            printf("moving = %d",moving);
            if (showPause)
            {
                if (vx != 0 && vy != 0)
                {
                    vxprev = vx;
                    vyprev = vy;
                }
                vx = 0;
                vy = 0;
            }
            
            if (showPause == false && moving == false)
            {
                vx = vxprev;
                vy = vyprev;
                printf("vxprev:%d, vyprev:%d \n",vxprev,vyprev);
            }
        }


//        if (showPause == false && moving == false)
//        {
//            vx = vxprev;
//            printf("point1");
//            vy = vyprev;
//            //            printf("but when paused vyprev %d \n",vyprev);
//        }
        
        for (int i = 0; i<brick_row; i++)
        {
            for (int j = 0; j < brick_col; j++)
            {
                int ox = bw * j;
                int oy = bh * i;
                
                //if the hit is successful
                if (0 != brick[i][j] && true == checkforcollision(xc, yc, ox, oy, bw, bh))
                {
                    int dx = xc - ox;
                    int dy = yc - oy;
                    
                    int x = prevx - xc;
                    int y =  prevy - yc;
                    
                    vy = -vy;
                    brick[i][j] = 0;
                    disappeared += 1;
                    
                }
            }
        }
        loopcount++;
    }
    if (cstate == 0)
    {
        if (FSKEY_P == key)
        {
            showPause = !showPause;
        }
        
        if (FSKEY_I == key)
        {
            showInstructions = !showInstructions;
        }
        
//        if (showPause)
//        {
//            showInstructions = false;
//            Pause();
//        }
//
//        if (showInstructions)
//        {
//            showPause = false;
//            Instructions();
//
//            //                printf("Pressed");
//        }
        
//        if (disappeared == brick_row * brick_col)
//        {
//            GameWon();
//        }
//        else if(cannons == 0)
//        {
//            GameOver();
//        }
    }
//    FsSwapBuffers();
//    FsSleep(15);
}
