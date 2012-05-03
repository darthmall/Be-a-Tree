//
//  blossom.h
//  Be A Tree
//
//  Created by Evan Sheehan on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Be_A_Tree_blossom_h
#define Be_A_Tree_blossom_h

#include "ofMain.h"

class blossom {

    float noise;
    float x;
    
public:
    ofPoint location;
    float r;

    blossom(float x, float y);
    
    void draw();
};

#endif
