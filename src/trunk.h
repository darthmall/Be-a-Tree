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
    
    trunk(float p_grow,
        float p_bifurcate,
        float thickness_factor,
        float min_length,
        float max_length,
        float max_size);

    ~trunk();

    void reset();
    void grow();
    void draw(ofxTrackedUser user, bool debug);
    
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
    
    void drawTrunk(ofxTrackedUser user, bool debug);
    void drawLimb(ofxLimb, float dangle, int depth, bool debug);
    float limbAngle(ofxLimb limb);
    float limbAngle(XnPoint3D start, XnPoint3D end);
    float limbLength(ofxLimb limb);
    float splitAngle(ofxLimb l1, ofxLimb l2);
    ofPoint jointStrut(ofxLimb first, ofxLimb second, float width);
    ofPoint jointStrut(ofxLimb limb, float width);

    float timestamp;
    
    float p_grow;
    float p_bifurcate;
    float thickness_factor;
    float min_length;
    float max_length;
    float max_size;

    map<XnSkeletonJoint, twig*> branches;
};

#endif
