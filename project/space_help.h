//
//  space_help.h
//  pheonix
//
//  Created by Chris Cortez on 11/19/18.
//  Copyright © 2018 Chris Cortez. All rights reserved.
//

#ifndef space_help_h
#define space_help_h

class Bullet {
protected:
    double *pos;
    bool fired;
public:
    Bullet();
    ~Bullet();
    
    //change
    void setPos(double x, double y);
    void Fire();
    void Reset();
    void Update(double speed);
    
    //access
    double getX() const;
    double getY() const;
    bool isFired() const;
    void Draw(int r, int g, int b) const;
};

class Creature {
private:
    int health;
    double *pos;
    double wid, hei;
    int num_bullets;
    
    
protected:
    Bullet *bullets;
    
public:
    Creature();
    ~Creature();
    
    //change
    void setHealth(int h);
    void setPosition(double x, double y);
    void setDim(double w, double h);
    void createBullets(const int count);
    
    //access
    int getHealth() const;
    double getX() const;
    double getY() const;
    double getWid() const;
    double getHei() const;
    int getNumBullets() const;
    
    //functions
    bool isDead() const;
    bool isHit(double bx, double by) const;
};


class Player_SI: public Creature {
private:
    int bullet_count;
    int speed;
    int invuln_timer;
    
public:
    Player_SI();
    ~Player_SI();
    void Create();
    void Delete();
    
    double getBulletX(int index) const;
    double getBulletY(int index) const;
    void resetBullet(int index);
    void resetAllBullets();
    
    void Shoot();
    void Move(int dir);
    void Update();
    
    bool isHit(int b_x, int b_y);
    
    void Draw() const;
};


class Enemy_A : public Creature {
private:
    int bullet_count;
    double angle;
    int fire_rate;
    
public:
    Enemy_A();
    ~Enemy_A();
    void Create(double x, double y, double w, double h, int fr);
    void Delete();
    
    void Shoot();
    void CheckPlayerHit(Player_SI& Play);
    void CheckEnemyHit(Player_SI& Play);
    void Update();
    
    void Draw() const;
    
};


class Minion : public Enemy_A {
public:
    Minion();
    ~Minion();
    void Create_M(double x, double y, double w, double h, int fr);
    
    void CheckEnemyHit(Player_SI& Play);
    void Draw() const;
};

class Boss : public Creature {
private:
    Minion *m;
    int bullet_count;
    int bul_timer;
    double speed;
    
public:
    Boss();
    ~Boss();
    void Create(double x, double y);
    void Delete();
    
    void Shoot();
    void CheckPlayerHit(Player_SI& Play);
    void CheckEnemyHit(Player_SI& Play, int &count);
    void Update();
    
    void Draw() const;
};


#endif /* space_help_h */
