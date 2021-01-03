//
//  frogger_class.h
//  frogger
//
//  Created by Samantha Ealy on 11/21/18.
//  Copyright © 2018 CMU. All rights reserved.
//

#ifndef frogger_class_h
#define frogger_class_h

#include <vector>
using namespace std;

class Rectangle
{
public:
    double x, y, w, h;
    int r, g, b;
    
    Rectangle();
    ~Rectangle();
    void Delete(void);
    void SetColor(int red, int green, int blue);
    bool Intersect(Rectangle obs);
};

class Player : public Rectangle
{
public:
    void Create(void);
    void Move(double dx, double dy);
    void Draw(void) const;
    int GetLane(void);
};

class Homes : public Rectangle
{
public:
    int state;
    void Create(int i, double spc);
    void Draw(void) const;
};


class Obstacles : public Rectangle
{
public:
    void Create(int row, int col, string o, double vel, double spc);
    void AssignColor(string type);
    void Move(double vel);
    void Draw(void) const;
};

class Car : public Obstacles
{
public:
    void Create(void);
};

class Lanes : public Rectangle
{
public:
    Obstacles* obs;
    vector<string> obsVec;
    double vel;
    int laneType, nObs;
    
    void Create(int i, int type);
    void CreateSafetyLane(void);
    void CreateHomeLane(void);
    void CreateCarLane(void);
    void CreateBusLane(void);
    void CreateLogLane(void);
    void SetCourse(int numObs, string obs[], string spc[]);
    void SwapString(string &a, string &b);
    void Move(void);
    void Delete(void);
    void Draw(void) const;
};

#endif /* frogger_class_h */
