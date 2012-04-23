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
    twig(float angle,
         float length,
         float p_grow,
         float p_bifurcate,
         float thickness_factor,
         float min_length,
         float max_length,
         float max_size);
    ~twig();
    
    void grow();
    void draw(float x, float y, float rotation);
    void draw(float x, float y);
    void clear();
    int depth();
    int size();

    void setPGrow(float p);
    float getPGrow();
    
    void setPBifurcate(float p);
    float getPBifurcate();
    
    void setThicknessFactor(float factor);
    float getThicknessFactor();
    
    void setMinLength(float l);
    float getMinLength();
    
    void setMaxLength(float l);
    float getMaxLength();
    
    void setMaxSize(float s);
    float getMaxSize();
    
    
protected:    
    float p_grow;
    float p_bifurcate;
    float thickness_factor;
    float min_length;
    float max_length;
    float angle;
    float length;
    float max_size;

    twig *left;
    twig *right;
};


#endif
