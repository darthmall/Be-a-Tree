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

    right_arm = new twig(0, 15, p_bifurcate, thickness_factor, 10, 15, 20);
    left_arm = new twig(0, 15, p_bifurcate, thickness_factor, 10, 15, 20);
    right_humorous = new twig(0, 15, p_bifurcate, thickness_factor, 15, 20, 20);
    left_humorous = new twig(0, 15, p_bifurcate, thickness_factor, 15, 20, 20);

    while (right_arm->size() < right_arm->getMaxSize()) {
        right_arm->grow();
    }
    right_arm->setGrown();
    
    while (left_arm->size() < left_arm->getMaxSize()) {
        left_arm->grow();
    }
    left_arm->setGrown();
    
    while (right_humorous->size() < right_humorous->getMaxSize()) {
        right_humorous->grow();
    }
    right_humorous->setGrown();

    while (left_humorous->size() < left_humorous->getMaxSize()) {
        left_humorous->grow();
    }
    left_humorous->setGrown();
    
    twig *t = right_humorous->find_node_at_depth(5);
    if (t) {
        t->concat(right_arm);
    } else {
        right_humorous->append(right_arm);
    }
    
    t = left_humorous->find_node_at_depth(5);
    if (t) {
        t->concat(left_arm);
    } else {
        left_humorous->append(left_humorous);
    }

    lshoulder = new twig(ofRandom(-5, 5), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (lshoulder->size() < lshoulder->getMaxSize()) {
        lshoulder->grow();
    }
    
    rshoulder = new twig(ofRandom(-5, 5), ofRandom(min_length, max_length),
                         p_bifurcate, thickness_factor, min_length,
                         max_length, max_size);
    while (rshoulder->size() < rshoulder->getMaxSize()) {
        rshoulder->grow();
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

    drawTrunk(user);
    right_arm->angle = ofRadToDeg(angle(user.right_upper_arm.position[0].X,
                                        user.right_upper_arm.position[0].Y,
                                        user.right_upper_arm.position[1].X,
                                        user.right_upper_arm.position[1].Y,
                                        user.right_lower_arm.position[1].X,
                                        user.right_lower_arm.position[1].Y)) - 180;
    left_arm->angle = 180 - ofRadToDeg(angle(user.left_upper_arm.position[0].X,
                                       user.left_upper_arm.position[0].Y,
                                       user.left_upper_arm.position[1].X,
                                       user.left_upper_arm.position[1].Y,
                                       user.left_lower_arm.position[1].X,
                                       user.left_lower_arm.position[1].Y));
    
    drawTwig(user.left_upper_arm, left_humorous, true);
    drawTwig(user.right_upper_arm, right_humorous, true);
    drawTwig(user.right_shoulder, rshoulder, true);
    drawTwig(user.left_shoulder, lshoulder, true);

//    float angle = ofRadToDeg(limbAngle(user.right_upper_arm)) + 90;
//    
//    ofPushMatrix();        
//    ofTranslate(user.left_lower_torso.position[0].X,
//                user.right_upper_arm.position[0].Y);
//    ofRotate(angle);
//    
//    right_humorous->draw();
//    
//    ofPopMatrix();
//    
//    ofPushMatrix();
//    angle = max(110.f, ofRadToDeg(limbAngle(user.left_upper_arm)) + 90);
//    ofTranslate(user.left_lower_torso.position[0].X,
//                user.left_upper_arm.position[0].Y);
//    ofRotate(angle);
//    left_humorous->draw();
//    ofPopMatrix();
//    
//    ofPopStyle();
}

void trunk::grow() {

}

void trunk::update() {
    float now = ofGetElapsedTimef();
    
    if (now - timestamp >= growthRate) {
        timestamp = now;
        lshoulder->update();
        rshoulder->update();
    }
}

void trunk::reset() {

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