//
//  ent.cpp
//  Be A Tree
//
//  Created by Evan Sheehan on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "trunk.h"
#include "util.h"
#include <iostream>

trunk::trunk(float p_grow,
         float p_bifurcate,
         float thickness_factor,
         float min_length,
         float max_length,
         float max_size) : p_grow(p_grow), p_bifurcate(p_bifurcate), thickness_factor(thickness_factor), min_length(min_length), max_length(max_length), max_size(max_size) {

    head = new twig(ofRandom(-5, 5), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    right_arm = new twig(0, 15, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    left_arm = new twig(0, 15, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    
    while (head->size() < head->getMaxSize()) {
        head->grow();
    }
    
    while (right_arm->size() < right_arm->getMaxSize()) {
        right_arm->grow();
    }
    
    while (left_arm->size() < left_arm->getMaxSize()) {
        left_arm->grow();
    }
    
    // FIXME: These parameters need tweaking
    left_shoulder = new twig(ofRandom(-185, -140), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    left_elbow = new twig(ofRandom(-140, -120), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    left_hip = new twig(ofRandom(-95, -85), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    right_shoulder = new twig(ofRandom(95, 40), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    right_elbow = new twig(ofRandom(20, 60), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    right_hip = new twig(ofRandom(120, 185), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
}

trunk::~trunk() {
    delete head;
    delete left_shoulder;
    delete right_shoulder;
    delete left_elbow;
    delete right_elbow;
    delete left_arm;
    delete right_arm;
    delete left_hip;
    delete right_hip;
}

void trunk::draw(ofxTrackedUser user) {
    ofPushStyle();
    ofFill();
    ofSetHexColor(0x5F6273);

    drawTwig(user.right_lower_arm, right_arm, true);
    drawTwig(user.left_lower_arm, left_arm, true);
    drawTwig(user.left_shoulder, left_shoulder);
    drawTwig(user.left_upper_arm, left_elbow);
    drawTwig(user.hip, left_hip);
    drawTwig(user.right_shoulder, right_shoulder);
    drawTwig(user.right_upper_arm, right_elbow);
    drawTwig(user.hip, right_hip, true);
}

void trunk::grow() {
    head->grow();
    left_shoulder->grow();
    left_elbow->grow();
    left_hip->grow();
    right_shoulder->grow();
    right_elbow->grow();
    right_hip->grow();
}

void trunk::reset() {
    head->clear();
    left_shoulder->clear();
    left_elbow->clear();
    left_hip->clear();
    right_shoulder->clear();
    right_elbow->clear();
    right_hip->clear();
}

void trunk::drawTrunk(ofxTrackedUser user) {

}

void trunk::drawTwig(ofxLimb limb, twig *t, bool asLimb) {
    if (limb.found) {
        float angle = DEGREES(limbAngle(limb));
        XnPoint3D origin;

        // If we draw the twig as the limb, position at the limb's origin, and
        // rotate an additional 90 degress to align the twig with the limb's
        // orientation.
        ofPushMatrix();
        if (asLimb) {
            origin = limb.position[0];
            angle += 90;
        } else {
            origin = limb.position[1];
        }
        
        ofTranslate(origin.X, origin.Y);
        ofRotate(angle);

        t->draw();

        ofPopMatrix();
    }
}

void trunk::setPGrow(float p) {
    p_grow = p;
}

void trunk::setPBifurcate(float p) {
    p_bifurcate = p;
}

void trunk::setThicknessFactor(float factor) {
    thickness_factor = factor;
}

void trunk::setMinLength(float l) {
    min_length = l;
}

void trunk::setMaxLength(float l) {
    max_length = l;
}

void trunk::setMaxSize(float s) {
    max_size = s;
}

float trunk::getPGrow() {
    return p_grow;
}

float trunk::getPBifurcate() {
    return p_bifurcate;
}

float trunk::getThicknessFactor() {
    return thickness_factor;
}

float trunk::getMinLength() {
    return min_length;
}

float trunk::getMaxLength() {
    return max_length;
}

float trunk::getMaxSize() {
    return max_size;
}