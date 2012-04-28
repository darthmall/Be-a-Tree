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
#include <cmath>

trunk::trunk(float p_bifurcate,
             float thickness_factor,
             float min_length,
             float max_length,
             float max_size) :
p_bifurcate(p_bifurcate),
thickness_factor(thickness_factor),
min_length(min_length),
max_length(max_length),
max_size(max_size),
growthRate(0.1) {

    neck = new twig(ofRandom(15, 40), ofRandom(min_length, max_length), p_bifurcate, thickness_factor, min_length, max_length, max_size);
    while (neck->size() < max_size) {
        neck->grow();
    }
                    
    right_arm = new twig(0, 15, p_bifurcate, thickness_factor, 10, 15, 20);
    while (right_arm->size() < right_arm->getMaxSize()) {
        right_arm->grow();
    }
    
    left_arm = new twig(0, 15, p_bifurcate, thickness_factor, 10, 15, 20);    
    while (left_arm->size() < left_arm->getMaxSize()) {
        left_arm->grow();
    }
    
    right_humorous = new twig(0, 15, p_bifurcate, thickness_factor, 15, 20, 20);    
    while (right_humorous->size() < right_humorous->getMaxSize()) {
        right_humorous->grow();
    }

    left_humorous = new twig(0, 15, p_bifurcate, thickness_factor, 15, 20, 20);
    while (left_humorous->size() < left_humorous->getMaxSize()) {
        left_humorous->grow();
    }

    rshoulder = new twig(ofRandom(20, 50), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (rshoulder->size() < max_size) {
        rshoulder->grow();
    }
    
    lshoulder = new twig(ofRandom(-50, -20), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (lshoulder->size() < max_size) {
        lshoulder->grow();
    }

    lhip = new twig(ofRandom(-90, -70), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (lhip->size() < lhip->getMaxSize()) {
        lhip->grow();
    }
    
    rhip = new twig(ofRandom(70, 90), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (rhip->size() < rhip->getMaxSize()) {
        rhip->grow();
    }

    timestamp = ofGetElapsedTimef();
}

trunk::~trunk() {
    delete left_arm;
    delete right_arm;
}

void trunk::draw(ofxTrackedUser user) {
    ofPushStyle();
    ofFill();
    ofSetHexColor(0x292E36);
    
    drawTwig(user.left_upper_arm, left_humorous, true);
    drawTwig(user.right_upper_arm, right_humorous, true);
    
    ofPushMatrix();
    ofTranslate(user.neck.position[0].X, user.neck.position[0].Y);
    neck->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(user.left_shoulder.position[1].X, user.left_shoulder.position[1].Y);
    lshoulder->draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(user.right_shoulder.position[1].X, user.right_shoulder.position[1].Y);
    rshoulder->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(user.hip.position[0].X, user.hip.position[0].Y);
    lhip->draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(user.hip.position[1].X, user.hip.position[1].Y);
    rhip->draw();
    ofPopMatrix();
}

void trunk::grow() {

}

void trunk::update() {
    float now = ofGetElapsedTimef();
    
    if (now - timestamp >= growthRate) {
        timestamp = now;
        neck->update();
        left_humorous->update();
        right_humorous->update();
        left_arm->update();
        right_arm->update();
        rshoulder->update();
        lshoulder->update();
        lhip->update();
        rhip->update();
    }
}

void trunk::reset() {
    reset(neck);
    reset(rshoulder);
    reset(lshoulder);
    reset(right_humorous);
    reset(left_humorous);
    reset(right_arm);
    reset(left_arm);
    reset(rhip);
    reset(lhip);
}

void trunk::reset(twig *t) {
    t->clear();
    while (t->size() < t->getMaxSize()) {
        t->grow();
    }
}

void trunk::drawTrunk(ofxTrackedUser user) {
    ofPushMatrix();
    ofTranslate(user.left_lower_torso.position[0].X,
                user.left_lower_torso.position[1].Y);
    ofPopMatrix();
}

void trunk::drawTwig(ofxLimb limb, twig *t, bool asLimb) {
    if (limb.found) {
        float angle = ofRadToDeg(limbAngle(limb));
        XnPoint3D origin;

        // If we draw the twig as the limb, position at the limb's origin and
        // rotate an additional 90 degress to align the twig with the limb's
        // orientation.
        if (asLimb) {
            origin = limb.position[0];
            angle += 90;
        } else {
            origin = limb.position[1];
        }

        ofPushMatrix();        
        ofTranslate(origin.X, origin.Y);
        ofRotate(angle);

        t->draw();

        ofPopMatrix();
    }
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