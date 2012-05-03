//
//  blossom.cpp
//  Be A Tree
//
//  Created by Evan Sheehan on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "blossom.h"

#include <iostream>

blossom::blossom(float x, float y) {
    location.x = x;
    location.y = y;
    r = ofRandom(4, 6);
    x = ofGetElapsedTimef();
}

void blossom::draw() {
    ofCircle(location.x + noise, location.y, r);
}