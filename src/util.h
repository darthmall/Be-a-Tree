//
//  util.h
//  Be A Tree
//
//  Created by Evan Sheehan on 4/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Be_A_Tree_util_h
#define Be_A_Tree_util_h

#include <cmath>

#include "ofxOpenNi.h"

float angle(float x1, float y1, float x2, float y2);

float limbAngle(ofxLimb limb);

vector<ofPoint> interpolate(float x1, float y1, float x2, float y2, int resolution);

#endif
