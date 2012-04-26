//
//  util.cpp
//  Be A Tree
//
//  Created by Evan Sheehan on 4/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "util.h"

float angle(float x1, float y1, float x2, float y2) {
    return atan((y2 - y1) / (x2 - x1));
}

float angle(float x1, float y1, float x2, float y2, float x3, float y3) {
    float a = abs(ofDist(x1, y1, x3, y3));
    float b = abs(ofDist(x2, y2, x1, y1));
    float c = abs(ofDist(x2, y2, x3, y3));

    return acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
}

float limbAngle(ofxLimb limb) {
    float theta = angle(limb.position[0].X, limb.position[0].Y,
                        limb.position[1].X, limb.position[1].Y);
    
    // Correct for OF's weird coordinate system with positive-y being down
    if (limb.position[1].X < limb.position[0].X) {
        // Quadrants II and III
        return M_PI + theta;
    }
    
    // Quadrants I and IV
    return theta;
}

vector<ofPoint> interpolate(float x1, float y1, float x2, float y2, int resolution) {
    float m = (y2 - y1) / (x2 - x1);
    float l = ofDist(x1, y1, x2, y2);
    float dx = (x2 - x1) / resolution;
    vector<ofPoint> points;
    
    for (int i = 0; i <= resolution; i++) {
        ofPoint p;
        
        p.x = x1 + (i * dx);
        p.y = y1 + (i * dx * m);
        
        points.push_back(p);
    }
    
    return points;
}