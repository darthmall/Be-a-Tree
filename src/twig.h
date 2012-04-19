//
//  twig.h
//  treeTest
//
//  Created by Evan Sheehan on 4/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef treeTest_twig_h
#define treeTest_twig_h

#include "ofMain.h"

class twig {
public:
    twig(float angle, float length, int depth);
    ~twig();
    
    void grow(float p_grow, float p_bifurcate, float l_bifurcate);
    void draw(float x, float y, float rotation);
    void draw(float x, float y);
    void clear();
    int height();
    int count();
    
protected:
    float angle;
    float length;
    int depth;
    bool bifurcated;

    vector<twig*> children;
};


#endif
