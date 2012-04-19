//
//  twig.cpp
//  treeTest
//
//  Created by Evan Sheehan on 4/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "twig.h"
#include <iostream>
#include <cmath>

twig::twig(float angle, float length, int depth) {
    this->angle = angle;
    this->length = length;
    this->depth = depth;
    bifurcated = false;
}

twig::~twig() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

void twig::grow(float p_grow, float p_bifurcate, float l_bifurcate) {
    for (vector<twig*>::iterator it = children.begin(); it < children.end(); it++) {
        (*it)->grow(p_grow, p_bifurcate, l_bifurcate);
    }
    
    if (children.size() < 1) {
        if (ofRandom(1) < p_grow) {
            twig* growth = new twig(ofRandom(-5, 5), ofRandom(20, 30), depth + 1);
            children.push_back(growth);
        }
    } else if (children.size() < 4) {
        if (height() >= l_bifurcate && ofRandom(1) < p_bifurcate) {
            float growth_angle = ofRandom(15, 40);
            
            // 50/50 chance to flip the sign on the angle
            if (ofRandom(1) < 0.5) {
                growth_angle *= -1;
            }

            bifurcated = true;
            twig* growth = new twig(growth_angle, ofRandom(30, 50), depth + 1);
            children.push_back(growth);            
        }
    }
}

void twig::draw(float x, float y, float rotation) {
    ofPushMatrix();

    ofPushMatrix();
    ofTranslate(x, y);
    ofRotate(rotation);
    ofRotate(angle);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(1);
    
    if (bifurcated) {
        ofSetHexColor(0x00ff00);
    } else {
        ofSetHexColor(0x000000);
    }
    
    ofLine(0, 0, 0, -length);
    
    ofPopStyle();
    
    for (vector<twig*>::iterator it = children.begin(); it < children.end(); it++) {
        (*it)->draw(0, -length);
    }
    
    ofPopMatrix();

}

void twig::draw(float x, float y) {
    draw(x, y, 0);
}

int twig::height() {
    int d = 0;
    
    for (int i = 0; i < children.size(); i++) {
        d = std::max(d, children[i]->height());
    }
    
    return d + 1;
}

int twig::count() {
    int c = 1;
    
    for (int i = 0; i < children.size(); i++) {
        c += children[i]->count();
    }
    
    return c;
}

void twig::clear() {
    children.clear();
}

