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

float limbAngle(ofxLimb limb) {
    float theta = angle(limb.position[0].X, limb.position[0].Y,
                        limb.position[1].X, limb.position[1].Y);
    
    // Correct for OF's weird coordinate system with positive-y being down
    if (limb.position[1].X < limb.position[0].X) {
        // Quadrants II and III
        return (limb.position[1].Y < limb.position[0].Y) ? 180 + abs(theta) : 180 - abs(theta);
        
    }
    
    // Quadrants I and IV
    return theta;
}
