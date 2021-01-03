//
//  demo.cpp
//  demo
//
//  Created by Chris Cortez on 10/15/18.
//  Copyright © 2018 Chris Cortez. All rights reserved.
//

#include <stdlib.h>
#include <string>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "SpaceInvaders.h"


//Base Bullet Class
Bullet::Bullet(){
    pos = new double[2];
    fired = false;
}
Bullet::~Bullet(){
    if (pos == nullptr) {
        delete [] pos;
    }
    pos = nullptr;
    fired = 0;
}
void Bullet::setPos(double x, double y) {
    if (pos==nullptr) {
        pos = new double[2];
    }
    pos[0] = x;
    pos[1] = y;
}
void Bullet::Fire() {
    fired = true;
}
void Bullet::Reset() {
    fired = false;
    pos[0] = 0;
    pos[1] = 0;
}
void Bullet::Update(double speed) {
    if (pos==nullptr) {
        return;
    }
    if (pos[0] <= 100 || pos[1] <=0 || pos[0] >= 700 || pos[1] >= 700) {
        Reset();
    }
    if (fired) {
        setPos(getX(), getY()+speed);
    }
}
double Bullet::getX() const {
    if (pos==nullptr) {
        return 0;
    }
    return pos[0];
}
double Bullet::getY() const {
    if (pos==nullptr) {
        return 0;
    }
    return pos[1];
}
bool Bullet::isFired() const {
    return fired;
}
void Bullet::Draw(int r, int g, int b) const {
    if (pos == nullptr) {
        return;
    }
    if (pos[0]<=100 || pos[1] <= 0) {
        return;
    }
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    
    glVertex2d(pos[0]+5, pos[1]);
    glVertex2d(pos[0]+5, pos[1]+15);
    glVertex2d(pos[0]-5, pos[1]+15);
    glVertex2d(pos[0]-5, pos[1]);
    
    glEnd();
}






// Base Creature class
Creature::Creature() {
    pos = new double[2];
    health = 0;
    wid = 0;
    hei = 0;
    bullets = nullptr;
    num_bullets = 0;
}
Creature::~Creature() {
    delete [] pos;
    pos = nullptr;
    if (bullets != nullptr) {
        delete [] bullets;
    }
    bullets = nullptr;
    num_bullets = 0;
    health = 0;
    wid = 0;
    hei = 0;
}
void Creature::setHealth(int h) {
    health = h;
}
void Creature::setPosition(double x, double y) {
    if (pos == nullptr) {
        return;
    }
    pos[0] = x;
    pos[1] = y;
}
void Creature::setDim(double w, double h) {
    wid = w;
    hei = h;
}
void Creature::createBullets(const int count) {
    num_bullets = count;
    if (bullets != nullptr) {
        delete [] bullets;
    }
    bullets = new Bullet[count];
}
int Creature::getHealth() const {
    return health;
}
double Creature::getX() const {
    if (pos == nullptr) {
        return 0;
    }
    return pos[0];
}
double Creature::getY() const {
    if (pos == nullptr) {
        return 0;
    }
    return pos[1];
}
double Creature::getWid() const {
    return wid;
}
double Creature::getHei() const {
    return hei;
}
int Creature::getNumBullets() const {
    return num_bullets;
}
bool Creature::isDead() const {
    return health <= 0;
}
bool Creature::isHit(double bx, double by) const {
    if (bx+5 >= getX()-wid/2 && bx-5 <= getX()+wid/2 &&
        by+15 >= getY()-hei/2 && by <= getY()+hei/2) {
        return true;
    }
    return false;
}









// Derived Player_SIClass
Player_SI::Player_SI() {
    bullet_count = 0;
    speed = 0;
    invuln_timer = 0;
}
Player_SI::~Player_SI() {
    Delete();
}
void Player_SI::Create() {
    createBullets(20);
    setPosition(400,600);
    setHealth(3);
    setDim(20, 25);
    speed = 2;
    invuln_timer = 0;
}
void Player_SI::Delete() {
    //printf("Game Over\n");
    bullet_count = 0;
    speed = 0;
    invuln_timer = 0;
}
double Player_SI::getBulletX(int index) const {
    if (bullets == nullptr) {
        return 0;
    }
    if (index < 0 || index >= getNumBullets()) {
        printf("Error, Player::getBullets was given improper index\n");
        return 0;
    }
    return bullets[index].getX();
}
double Player_SI::getBulletY(int index) const {
    if (bullets == nullptr) {
        return 0;
    }
    if (index < 0 || index >= getNumBullets()) {
        printf("Error, Player::getBullets was given improper index\n");
        return 0;
    }
    return bullets[index].getY();
}
void Player_SI::resetBullet(int index) {
    if (bullets == nullptr) {
        return;
    }
    
    bullets[index].Reset();
    
}
void Player_SI::resetAllBullets() {
    for (int i=0; i<getNumBullets(); i++) {
        resetBullet(i);
    }
}
void Player_SI::Shoot() {
    
    if (bullets == nullptr) {
        return;
    }
    if (bullets[bullet_count].isFired()) {
        return;
    }
    bullets[bullet_count].setPos(getX(), getY());
    bullets[bullet_count].Fire();
    bullet_count = (bullet_count+1)%getNumBullets();
}
void Player_SI::Move(int dir) {
    switch (dir) {
        case 0:
            break;
            
        case 1:
            if (getY() <= 500) {
                return;
            }
            setPosition(getX(), getY()-speed);
            break;
            
        case 2:
            if (getX() >= 685) {
                return;
            }
            setPosition(getX()+speed, getY());
            break;
            
        case 3:
            if (getY() >= 670) {
                return;
            }
            setPosition(getX(), getY()+speed);
            break;
            
        case 4:
            if (getX() <= 115) {
                return;
            }
            setPosition(getX()-speed, getY());
            break;
            
        default:
            break;
    }
}
bool Player_SI::isHit(int b_x, int b_y) {
    if (invuln_timer > 0) {
        return false;
    }
    if (Creature::isHit(b_x, b_y)) {
        invuln_timer = 40;
        return true;
    }
    return false;
}
void Player_SI::Update() {
    if (isDead()) {
        Delete();
        return;
    }
    
    for (int i=0; i<getNumBullets(); i++) {
        if (bullets == nullptr) {
            break;
        }
        if (bullets[i].isFired()) {
            bullets[i].Update(-5);
        }
    }
    if (invuln_timer > 0) {
        invuln_timer--;
    }
}
void Player_SI::Draw() const {
   
    for (int i=0; i<getNumBullets(); i++) {
        if (bullets == nullptr) {
            break;
        }
        bullets[i].Draw(230, 0, 230);
    }
    
    if ((invuln_timer/5)%2 == 1) {
        return;
    }
    glBegin(GL_TRIANGLES);
    glColor3ub(255, 255, 255);
    
    glVertex2d(getX(), getY()-getHei()/2);
    glVertex2d(getX()+getWid()/2, getY()+getHei()/2);
    glVertex2d(getX()-getWid()/2, getY()+getHei()/2);
    
    glEnd();
   
}





//Derived Basic Enemy Class
Enemy_A::Enemy_A() {
    bullet_count = 0;
    angle = 0;
    fire_rate = 0;
}
Enemy_A::~Enemy_A() {
    Delete();
}
void Enemy_A::Create(double x, double y, double w, double h, int fr) {
    createBullets(3);
    setPosition(x, y);
    setHealth(1);
    bullet_count = 0;
    angle = 0;
    fire_rate = fr;
    setDim(w, h);
}
void Enemy_A::Delete() {
    bullet_count = 0;
    angle = 0;
    fire_rate = 0;
}
void Enemy_A::Shoot() {
    if (bullets == nullptr) {
        return;
    }
    if (bullets[bullet_count].isFired()) {
        return;
    }
    bullets[bullet_count].setPos(getX(), getY()+getHei()/2.0);
    bullets[bullet_count].Fire();
    
    
    bullet_count = (bullet_count+1)%getNumBullets();
}
void Enemy_A::CheckPlayerHit(Player_SI &Play) {
    if (bullets == nullptr) {
        return;
    }
    for (int i=0; i<getNumBullets(); i++) {
        if (!bullets[i].isFired()) {
            continue;
        }
        if (Play.isHit(bullets[i].getX(), bullets[i].getY())) {
            Play.setHealth(Play.getHealth()-1);
            
            bullets[i].Reset();
        }
    }
}
void Enemy_A::CheckEnemyHit(Player_SI& Play) {
    if (Play.isDead()) {
        return;
    }
    for (int i=0; i<Play.getNumBullets(); i++) {
        if(isHit(Play.getBulletX(i), Play.getBulletY(i))) {
            setHealth(getHealth()-1);
            Play.resetBullet(i);
            return;
        }
    }
}
void Enemy_A::Update() {
    
    double r = 1;
    setPosition(getX()+r*cos(angle), getY()+r*sin(angle));
    angle += M_PI/64;
    
    int ran;
    ran = rand();
    if (ran%fire_rate == 0) {
        Shoot();
    }
    
    for (int i=0; i<getNumBullets(); i++) {
        if (bullets[i].isFired()) {
            bullets[i].Update(3);
        }
    }
    if (isDead()) {
        setPosition(-100, 0);
    }
}
void Enemy_A::Draw() const {
    
    for (int i=0; i<getNumBullets(); i++) {
        bullets[i].Draw(255, 0, 0);
    }
    if (!isDead()) {
        glBegin(GL_QUADS);
        glColor3ub(255, 255, 255);
        
        glVertex2d(getX()-getWid()/2, getY()-getHei()/2);
        glVertex2d(getX()-getWid()/2, getY()+getHei()/2);
        glVertex2d(getX()+getWid()/2, getY()+getHei()/2);
        glVertex2d(getX()+getWid()/2, getY()-getHei()/2);
        
        glEnd();
    }
}



//Derived Minion Class
Minion::Minion() {
}
Minion::~Minion() {
}
void Minion::Create_M(double x, double y, double w, double h, int fr) {
    Create(x, y, w, h, fr);
    setHealth(3);
}
void Minion::CheckEnemyHit(Player_SI& Play) {
    for (int i=0; i<Play.getNumBullets(); i++) {
        if(isHit(Play.getBulletX(i), Play.getBulletY(i))) {
            setHealth(getHealth()-1);
            Play.resetBullet(i);
            return;
        }
    }
}
void Minion::Draw() const {
    for (int i=0; i<getNumBullets(); i++) {
        if (bullets == nullptr) {
            break;
        }
        bullets[i].Draw(255, 0, 0);
    }
    if (!isDead()) {
        glBegin(GL_TRIANGLE_FAN);
        
        switch (getHealth()) {
            case 3:
                glColor3ub(255, 255, 255);
                break;
                
            case 2:
                glColor3ub(200, 200, 200);
                break;
                
            case 1:
                glColor3ub(100, 100, 100);
                break;
                
            default:
                glColor3ub(255, 255, 255);
                break;
        };
        
        glVertex2d(getX(), getY());
        for(int i=0; i<=360; i+=30)
        {
            double a=(double)i*M_PI/180.0;
            double s=sin(a);
            double c=cos(a);
            
            double xx=(double)getX()+c*getWid()/2;
            double yy=(double)getY()+s*getHei()/2;
            glVertex2d(xx,yy);
        }
        
        glEnd();
    }
}



//Derived Boss Class
Boss::Boss() {
    m = nullptr;
    bullet_count = 0;
    speed = 0;
    bul_timer = 0;
}
Boss::~Boss() {
    Delete();
}
void Boss::Create(double x, double y) {
    setPosition(x, y);
    setHealth(20);
    setDim(100, 50);
    m = new Minion[10];
    createBullets(40);
    for (int i=0; i<10; i++) {
        m[i].Create_M(100*(double)(i%5)+190, 80.0*(double)(i/5)+190, 50, 50, 100);
    }
    speed = 1;
}
void Boss::Delete() {
    setPosition(0, 0);
    setHealth(0);
    delete [] m;
    m = nullptr;
    bullet_count = 0;
    speed = 0;
    bul_timer = 0;
    for (int i=0; i<getNumBullets(); i++) {
        bullets[i].Reset();
    }
}
void Boss::Shoot() {
    if (bullets==nullptr) {
        return;
    }
    if (bullets[bullet_count].isFired()) {
        return;
    }
    bullets[bullet_count].setPos(getX()-getWid()/2, getY());
    bullets[bullet_count].Fire();
    bullet_count = (bullet_count+1)%getNumBullets();
    
    if (bullets[bullet_count].isFired()) {
        return;
    }
    bullets[bullet_count].setPos(getX()+getWid()/2, getY());
    bullets[bullet_count].Fire();
    bullet_count = (bullet_count+1)%getNumBullets();
}
void Boss::CheckPlayerHit(Player_SI& Play) {
    
    for (int i=0; i<getNumBullets(); i++) {
        if (!bullets[i].isFired()) {
            continue;
        }
        if (Play.isHit(bullets[i].getX(), bullets[i].getY())) {
            Play.setHealth(Play.getHealth()-1);
            
            bullets[i].Reset();
        }
    }
    
    
    for (int i=0; i<10; i++) {
        if(m == nullptr) {
            break;
        }
        m[i].CheckPlayerHit(Play);
    }
}
void Boss::CheckEnemyHit(Player_SI &Play, int& count) {
    for (int i=0; i<10; i++) {
        if (m == nullptr) {
            break;
        }
        if (m[i].isDead()) {
            continue;
        }
        m[i].CheckEnemyHit(Play);
        if (m[i].isDead()) {
            count--;
        }
    }
    if (!isDead()) {
        for (int i=0; i<Play.getNumBullets(); i++) {
            if (isHit(Play.getBulletX(i),Play.getBulletY(i))) {
                setHealth(getHealth()-1);
                Play.resetBullet(i);
            }
        }
        if (isDead()) {
            count--;
        }
    }
}
void Boss::Update() {
    bul_timer++;
    for (int i=0; i<10; i++) {
        if (m == nullptr) {
            break;
        }
        m[i].Update();
    }
    
    for (int i=0; i<getNumBullets(); i++) {
        if (bullets[i].isFired()) {
            bullets[i].Update(3);
        }
    }
    
    if (isDead()) {
        return;
    }
    
    if (bul_timer %30 == 0) {
        Shoot();
    }
    setPosition(getX()+speed, getY());
    if (getX() >= 625 || getX() <= 175) {
        speed = -1*speed;
    }
}
void Boss::Draw() const {
    for (int i=0; i<getNumBullets(); i++) {
        bullets[i].Draw(255, 0, 0);
    }
    for (int i=0; i<10; i++) {
        if (m == nullptr) {
            break;
        }
        m[i].Draw();
    }
    if (!isDead()) {
        glBegin(GL_TRIANGLE_FAN);
        
        switch ((getHealth()+4)/5) {
            case 4:
                glColor3ub(255, 255, 255);
                break;
                
            case 3:
                glColor3ub(0, 255, 255);
                break;
                
            case 2:
                glColor3ub(0, 0, 200);
                break;
                
            case 1:
                glColor3ub(0, 100, 0);
                break;
                
            default:
                glColor3ub(255, 255, 255);
                break;
        };
        
        glVertex2d(getX(), getY());
        for(int i=0; i<=360; i+=20)
        {
            double a=(double)i*M_PI/180.0;
            double s=sin(a);
            double c=cos(a);
            
            double xx=(double)getX()+c*getWid()/2;
            double yy=(double)getY()+s*getHei()/2;
            glVertex2d(xx,yy);
        }
        
        glEnd();
    }
}





Phoenix::Phoenix() {
    e_A = nullptr;
    length_e_A = 0;
    paused = false;
    timer = 0;
    text = true;
    e_left = 0;
    level = 0;
}
Phoenix::~Phoenix() {
    Delete();
}
void Phoenix::Delete() {
    play.Delete();
    delete [] e_A;
    e_A = nullptr;
    length_e_A = 0;
    timer = 0;
    paused = 0;
    B.Delete();
    text = 0;
    e_left = 0;
    level = 0;
}
void Phoenix::Draw() const {
    DrawBackground();
    if (e_A != nullptr) {
        for (int i=0; i<length_e_A; i++) {
            e_A[i].Draw();
        }
    }
    B.Draw();
    play.Draw();
    DrawOverlay();
    DrawTitle();
}
void Phoenix::Create() {
    play.Create();
}
void Phoenix::Play(int key) {
    
    //check for cutscene
    if (timer == 300) {
        text = false;
        level = 1;
        LevelStart();
        //lvl_timer = 0;
        timer = 998;
    }
    
    if (timer >= 1100 && timer <= 1200) {
        text = false;
        level = 2;
        LevelStart();
        //lvl_timer = 0;
        timer = 1998;
    }
    
    if (timer >= 2100 && timer <= 2200) {
        text = false;
        level = 3;
        LevelStart();
        //lvl_timer = 0;
        timer = 2998;
    }
    
    if (key == FSKEY_3) {
        level = 3;
        LevelStart();
        text = false;
        timer = 2998;
    }
    
    
    if (key == FSKEY_P) {
        Pause();
    }
    
    if(play.isDead()) {
        return;
    }
    
    if (text) {
        if (!paused) {
            timer++;
        }
        return;
    }
    
    //--------------Play game-----------
    //lvl_timer++;
    play.Move(0);
        
    if (key == FSKEY_SPACE) {
        play.Shoot();
    }
    
    //allow player_SI movement if alive
    if (!play.isDead()) {
        if (FsGetKeyState(FSKEY_UP)) {
            play.Move(1);
        }
        if (FsGetKeyState(FSKEY_RIGHT)) {
            play.Move(2);
        }
        if (FsGetKeyState(FSKEY_DOWN)) {
            play.Move(3);
        }
        if (FsGetKeyState(FSKEY_LEFT)) {
            play.Move(4);
        }
    }
    //printf("Player Health: %d\n",P.getHealth());
    //printf("Enemies left: %d\n",num_e);
    //printf("Score: %d\n", score);
    
    //general update step
    play.Update();
    for (int i=0; i<length_e_A; i++) {
        if (e_A == nullptr) {
            break;
        }
        e_A[i].Update();
        e_A[i].CheckPlayerHit(play);
        
        if (e_A[i].isDead()) {
            continue;
        }
        e_A[i].CheckEnemyHit(play);
        if(e_A[i].isDead()) {
            e_left--;
        }
    }
    
    B.Update();
    B.CheckPlayerHit(play);
    B.CheckEnemyHit(play,e_left);
    
    //check if cleared level
    if (e_left == 0) {
        text = true;
        play.resetAllBullets();
        timer++;
        delete [] e_A;
        e_A = nullptr;
        B.Delete();
    }
}
void Phoenix::Pause() {
    if (!paused) {
        text = true;
        paused = true;
    }
    else {
        text = false;
        paused = false;
    }
}
void Phoenix::Instructions(){
    
}
void Phoenix::DrawBackground() const {
    glBegin(GL_QUADS);
    
    glColor3d(.5, .5,.5);
    
    glVertex2d(0, 0);
    glVertex2d(0, 800);
    glVertex2d(800, 800);
    glVertex2d(800, 0);
    
    glColor3ub(0, 0, 0);
    
    glVertex2d(100, 0);
    glVertex2d(100, 700);
    glVertex2d(700, 700);
    glVertex2d(700, 0);
    
    glEnd();
    
}
void Phoenix::DrawOverlay() const {
    
    glBegin(GL_QUADS);
    glColor3d(1, 1, 1);
    
    glVertex2d(100, 700);
    glVertex2d(100, 800);
    glVertex2d(700, 800);
    glVertex2d(700, 700);
    
    glEnd();
    
    glColor3d(0, 0, 0);
    glRasterPos2d(110.0, 730.0);
    YsGlDrawFontBitmap16x20("Player Health:");
    
    const std::string str = std::to_string(play.getHealth());
    char const *ch = str.c_str();
    glRasterPos2d(350.0, 730.0);
    YsGlDrawFontBitmap16x20(ch);
    
    glColor3d(0, 0, 0);
    glRasterPos2d(580.0, 730.0);
    YsGlDrawFontBitmap16x20("Level:");
    
    const std::string stri = std::to_string(level);
    char const *chr = stri.c_str();
    glRasterPos2d(680.0, 730.0);
    YsGlDrawFontBitmap16x20(chr);
}

void Phoenix::LevelStart() {
    play.resetAllBullets();
    switch (level) {
        case 1:
            e_A = new Enemy_A[20];
            length_e_A = 20;
            for (int i=0; i<length_e_A; i++) {
                e_A[i].Create(100*(double)(i%5)+202.5, 80.0*(double)(i/5)+42.5, 25, 25, 300);
            }
            e_left = 20;
            break;
            
        case 2:
            e_A = new Enemy_A[20];
            length_e_A = 20;
            for (int i=0; i<length_e_A; i++) {
                e_A[i].Create(100*(double)(i%5)+202.5, 80.0*(double)(i/5)+42.5, 25, 25, 100);
            }
            e_left = 20;
            break;
            
        case 3:
            B.Create(400, 75);
            e_left = 11;
            break;
            
        default:
            break;
    };
}
void Phoenix::DrawTitle() const {
    glColor3d(1, 1, 1);
    
    if (paused) {
        glRasterPos2d(200, 150);
        YsGlDrawFontBitmap20x28("Paused");
        DrawInstructions();
        return;
    }
    
    if (play.isDead()) {
        glRasterPos2d(200.0,400.0);
        YsGlDrawFontBitmap20x28("Game Over");
    }
    
    if (timer < 200 ) {
        glRasterPos2d(200.0,150.0);
        YsGlDrawFontBitmap20x28("Phoenix");
        DrawInstructions();
    }
    if (timer >= 200 && timer < 300) {
        glRasterPos2d(200.0,400.0);
        YsGlDrawFontBitmap20x28("Level 1:");
        glRasterPos2d(200.0,450.0);
        YsGlDrawFontBitmap16x20("Simple Enemies, Low Fire Rate");
    }
    
    if (timer >= 1000 && timer <= 1100) {
        glRasterPos2d(200.0,400.0);
        YsGlDrawFontBitmap20x28("Level 2:");
        glRasterPos2d(200.0,450.0);
        YsGlDrawFontBitmap16x20("Simple Enemies, High Fire Rate");
    }
    
    if (timer >= 2000 && timer <= 2100) {
        glRasterPos2d(200.0,400.0);
        YsGlDrawFontBitmap20x28("Level 3:");
        glRasterPos2d(200.0,450.0);
        YsGlDrawFontBitmap16x20("Boss Fight!!");
    }
    
    if (timer >= 3000) {
        glRasterPos2d(200.0,400.0);
        YsGlDrawFontBitmap20x28("A WINNER IS YOU!");
    }
}

void Phoenix::DrawInstructions() const {
    glColor3d(1, 1, 1);
    glRasterPos2d(150.0,300.0);
    YsGlDrawFontBitmap16x20("Hit P pause/unpause");
    glRasterPos2d(150.0,350.0);
    YsGlDrawFontBitmap16x20("Use the Arrows to Move");
    glRasterPos2d(150.0,400.0);
    YsGlDrawFontBitmap16x20("Use Space to Shoot");
    glRasterPos2d(150.0,450.0);
    YsGlDrawFontBitmap16x20("Use ESC to Return to Arcade");
}





