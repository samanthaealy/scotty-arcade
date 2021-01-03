//
//  frogger.h
//  frogger
//
//  Created by Samantha Ealy on 11/21/18.
//  Copyright © 2018 CMU. All rights reserved.
//

#include "Home Screen.h"
#include "frogger_class.h"

#ifndef frogger_h
#define frogger_h

class Frogger : public Game
{
private:
    Player frog;
    Lanes lane[14];
    Homes home[5];
    bool paused, gameOver, text, levelUp;
    unsigned long t0, timer;
    int nLanes, nLives, nHomes;
    int score, level;
    
    void LevelStart(void);
    void DrawBackground(void) const;
    void DrawOverlay(void) const;
    void DrawTitle(void) const;
    void DrawInstructions(void) const;
    void DrawGameOver(void) const;
    
public:
    Frogger();
    ~Frogger();
    void Delete(void);
    
    void Create(void);
    void Draw(void) const;
    void Play(int key);
    void Pause(void);
};


#endif /* frogger_h */
