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
#include "blossom.h"
#include <cmath>

class twig {
public:
    friend class trunk;

    twig(float angle,
         float length,
         float p_bifurcate,
         float thickness_scale,
         float thickness_factor,
         float min_length,
         float max_length,
         float max_size);
    ~twig();
    
    void grow();
    void draw();
    void clear();
    int depth();
    int size();
    
    void update(bool shrink);
    void append(twig *transplant);
    void concat(twig *transplant);
    twig *find_node_at_depth(int d);
    
    void setGrown();

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
    
    void setScale(float s);
    float getScale();

protected:    
    float p_bifurcate;
    float thickness_factor;
    float min_length;
    float max_length;
    float angle;
    float length;
    float max_size;
    float scale;
    
    bool grown;

    ofPoint contour[4];

    twig *parent;
    twig *left;
    twig *right;
    
    twig *find_node_at_depth(int d, int current_depth);
    
    vector<blossom *> blossoms;
};


#endif
