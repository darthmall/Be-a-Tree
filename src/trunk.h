//
//  ent.h
//  Be A Tree
//
//  Created by Evan Sheehan on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Be_A_Tree_ent_h
#define Be_A_Tree_ent_h

#include "ofxOpenNI.h"
#include "twig.h"

class trunk {

public:
    
    trunk(float p_bifurcate,
        float thickness_factor,
        float min_length,
        float max_length,
        float max_size);

    ~trunk();

    void update();
    void reset();
    void grow();
    void draw(ofxTrackedUser user);
    
    void setPBifurcate(float p);
    void setThicknessFactor(float factor);
    void setMinLength(float l);
    void setMaxLength(float l);
    void setMaxSize(float s);

    float getPBifurcate();
    float getThicknessFactor();
    float getMinLength();
    float getMaxLength();
    float getMaxSize();

    float growthRate;

private:
    void drawTrunk(ofxTrackedUser user);
    void drawTwig(ofxLimb limb, twig *t, bool asLimb=false);

    float timestamp;
    float p_bifurcate;
    float thickness_factor;
    float min_length;
    float max_length;
    float max_size;
    
    twig *main;
    twig *right_humorous;
    twig *left_humorous;
    twig *right_arm;
    twig *left_arm;

};

#endif
