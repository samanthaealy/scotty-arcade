//
//  Phoenix.h
//  Phoenix
//
//  Created by Chris Cortez on 11/18/18.
//  Copyright © 2018 Chris Cortez. All rights reserved.
//

#include "space_help.h"
#include "Home Screen.h" 

#ifndef Phoenix_h
#define Phoenix_h


class Phoenix : public Game {
private:
    Player_SI play;
    bool paused;
    bool text;
    int timer;
    Enemy_A* e_A;
    int length_e_A;
    Boss B;
    int e_left;
    int level;
    
    void LevelStart();
    void DrawBackground() const;
    void DrawOverlay() const;
    void DrawTitle() const;
    void DrawInstructions() const;
    
    
public:
    Phoenix();
    ~Phoenix();
    void Delete();
    
    void Draw() const;
    void Create();
    void Play(int key);
    void Pause();
    void Instructions();
};

#endif /* Phoenix_h */
