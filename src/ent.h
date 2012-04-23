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

class ent {

public:
    
    ent(ofxOpenNIUser user,
        float p_grow,
        float p_bifurcate,
        float thickness_factor,
        float min_length,
        float max_length,
        float max_size);

    ~ent();

    void reset();
    void grow();
    void draw(ofxOpenNIUser user);
    
    void setPGrow(float p);
    void setPBifurcate(float p);
    void setThicknessFactor(float factor);
    void setMinLength(float l);
    void setMaxLength(float l);
    void setMaxSize(float s);
    
    float getPGrow();
    float getPBifurcate();
    float getThicknessFactor();
    float getMinLength();
    float getMaxLength();
    float getMaxSize();

private:
    void drawTorso(ofxOpenNIUser user);
    void drawLimb(ofxOpenNIUser user, Limb limbType, float w);
    void growTips(vector<twig> & appendage);
    float calculateTrunkWidth(ofxOpenNIUser user);
    float limbAngle(ofxOpenNILimb limb);
    float limbAngle(ofxOpenNIJoint start, ofxOpenNIJoint end);
    float limbAngle(ofPoint start, ofPoint end);

    float timestamp;
    
    float p_grow;
    float p_bifurcate;
    float thickness_factor;
    float min_length;
    float max_length;
    float max_size;

    map<Joint, twig*> joints;
};

#endif
