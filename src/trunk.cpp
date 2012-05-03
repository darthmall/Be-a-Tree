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
             float scale,
             float thickness_factor,
             float min_length,
             float max_length,
             float max_size,
             float growth_rate) :
p_bifurcate(p_bifurcate),
scale(scale),
thickness_factor(thickness_factor),
min_length(min_length),
max_length(max_length),
max_size(max_size),
growthRate(growth_rate) {
    
    head = new twig(ofRandom(-5, 5), ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, max_size);
    while (head->size() < max_size) {
        head->grow();
    }

    neck = new twig(ofRandom(15, 40), ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, max_size);
    while (neck->size() < max_size) {
        neck->grow();
    }
                    
    right_arm = new twig(-5, ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, 10, 15, 20);
    while (right_arm->size() < right_arm->getMaxSize()) {
        right_arm->grow();
    }
    
    left_arm = new twig(5, 15, p_bifurcate, scale, thickness_factor, 10, 15, 20);    
    while (left_arm->size() < left_arm->getMaxSize()) {
        left_arm->grow();
    }
    
    right_humorous = new twig(10, ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, 20);    
    while (right_humorous->size() < right_humorous->getMaxSize()) {
        right_humorous->grow();
    }

    left_humorous = new twig(-10, ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, 20);
    while (left_humorous->size() < left_humorous->getMaxSize()) {
        left_humorous->grow();
    }

    rshoulder = new twig(ofRandom(40, 50), ofRandom(min_length, max_length),
                         p_bifurcate, scale, thickness_factor, min_length,
                         max_length, max_size);
    while (rshoulder->size() < max_size) {
        rshoulder->grow();
    }
    
    lshoulder = new twig(ofRandom(-50, -40), ofRandom(min_length, max_length),
                         p_bifurcate, scale, thickness_factor, min_length,
                         max_length, max_size);
    while (lshoulder->size() < max_size) {
        lshoulder->grow();
    }

    lhip = new twig(ofRandom(-90, -70), ofRandom(min_length, max_length),
                         p_bifurcate, scale, thickness_factor, min_length,
                         max_length, max_size);
    while (lhip->size() < lhip->getMaxSize()) {
        lhip->grow();
    }
    
    rhip = new twig(ofRandom(70, 90), ofRandom(min_length, max_length),
                         p_bifurcate, scale, thickness_factor, min_length,
                         max_length, max_size);
    while (rhip->size() < rhip->getMaxSize()) {
        rhip->grow();
    }

    timestamp = ofGetElapsedTimef();
}

trunk::~trunk() {
    delete head;
    head = NULL;

    delete neck;
    neck = NULL;

    delete right_humorous;
    right_humorous = NULL;

    delete left_humorous;
    left_humorous = NULL;

    delete left_arm;
    left_arm = NULL;

    delete right_arm;
    right_arm = NULL;

    delete lhip;
    lhip = NULL;

    delete rhip;
    rhip = NULL;

    delete lshoulder;
    lshoulder = NULL;

    delete rshoulder;
    rshoulder = NULL;
}

void trunk::draw(ofxTrackedUser user) {
    ofPushStyle();
    ofFill();
    ofSetHexColor(0x2B1702);
    
    float angle = limbAngle(user.left_upper_arm);
    float l = ofDist(user.left_upper_arm.position[0].X,
                     user.left_upper_arm.position[0].Y,
                     user.left_upper_arm.position[1].X,
                     user.left_upper_arm.position[1].Y);
    ofPushMatrix();
    ofTranslate(user.left_upper_arm.position[0].X + ((l / 2) * cos(angle)),
                user.left_upper_arm.position[0].Y + ((l / 2) * sin(angle)));
    ofRotate(ofRadToDeg(angle) - 270);
    left_humorous->draw();
    ofPopMatrix();
    
    angle = limbAngle(user.right_upper_arm);
    l = ofDist(user.right_upper_arm.position[0].X,
               user.right_upper_arm.position[0].Y,
               user.right_upper_arm.position[1].X,
               user.right_upper_arm.position[1].Y);

    ofPushMatrix();
    ofTranslate(user.right_upper_arm.position[0].X + ((l / 2) * cos(angle)),
                user.right_upper_arm.position[0].Y + ((l / 2) * sin(angle)));
    ofRotate(ofRadToDeg(angle) + 90);
    right_humorous->draw();
    ofPopMatrix();

    angle = limbAngle(user.neck);
    l = ofDist(user.neck.position[0].X,
               user.neck.position[0].Y,
               user.neck.position[1].X,
               user.neck.position[1].Y);
    
    ofPushMatrix();
    ofTranslate(user.neck.position[0].X + ((l / 2.f) * cos(angle)),
                user.neck.position[0].Y + ((l / 2.f) * sin(angle)));
    ofRotate(ofRadToDeg(angle) - 90);
    head->draw();
    ofPopMatrix();

    drawTwig(user.left_lower_arm, left_arm, true);
    drawTwig(user.right_lower_arm, right_arm, true);
    
//    ofPushMatrix();
//    ofTranslate(user.neck.position[1].X, user.neck.position[1].Y);
//    neck->draw();
//    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(user.left_shoulder.position[0].X, user.left_shoulder.position[0].Y);
    lshoulder->draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(user.right_shoulder.position[0].X, user.right_shoulder.position[0].Y);
    rshoulder->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(user.hip.position[0].X, user.hip.position[0].Y - 38);
    lhip->draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(user.hip.position[1].X, user.hip.position[1].Y - 20);
    rhip->draw();
    ofPopMatrix();
    
    ofPopStyle();
}

void trunk::grow() {

}

void trunk::update(bool shrink) {
    float now = ofGetElapsedTimef();
    float diff = (shrink) ? (growthRate / 3.f) : growthRate;
    float dt = now - timestamp;
    
    if (dt >= diff) {
        timestamp = now;
        head->update(shrink);
        neck->update(shrink);
        left_humorous->update(shrink);
        right_humorous->update(shrink);
        left_arm->update(shrink);
        right_arm->update(shrink);
        rshoulder->update(shrink);
        lshoulder->update(shrink);
        lhip->update(shrink);
        rhip->update(shrink);
    }
}

void trunk::reset() {
    reset(head);
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
            origin = limb.position[1];
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
    
    if (head) {
        head->setPBifurcate(p);
    }
    
    if (neck) {
        neck->setPBifurcate(p);
    }
    
    if (right_humorous) {
        right_humorous->setPBifurcate(p);
    }
    
    if (left_humorous) {
        left_humorous->setPBifurcate(p);
    }
    
    if (right_arm) {
        right_arm->setPBifurcate(p);
    }
    
    if (left_arm) {
        left_arm->setPBifurcate(p);
    }
    
    if (rhip) {
        rhip->setPBifurcate(p);
    }
    
    if (lhip) {
        lhip->setPBifurcate(p);
    }
    
    if (rshoulder) {
        rshoulder->setPBifurcate(p);
    }
    
    if (lshoulder) {
        lshoulder->setPBifurcate(p);
    }
}

void trunk::setThicknessFactor(float factor) {
    thickness_factor = factor;
    
    if (head) {
        head->setThicknessFactor(factor);
    }
    
    if (neck) {
        neck->setThicknessFactor(factor);
    }
    
    if (right_humorous) {
        right_humorous->setThicknessFactor(factor);
    }
    
    if (left_humorous) {
        left_humorous->setThicknessFactor(factor);
    }
    
    if (right_arm) {
        right_arm->setThicknessFactor(factor);
    }
    
    if (left_arm) {
        left_arm->setThicknessFactor(factor);
    }
    
    if (rhip) {
        rhip->setThicknessFactor(factor);
    }
    
    if (lhip) {
        lhip->setThicknessFactor(factor);
    }
    
    if (rshoulder) {
        rshoulder->setThicknessFactor(factor);
    }
    
    if (lshoulder) {
        lshoulder->setThicknessFactor(factor);
    }
}

void trunk::setMinLength(float l) {
    min_length = l;
    
    if (head) {
        head->setMinLength(l);
    }
    
    if (neck) {
        neck->setMinLength(l);
    }
    
    if (right_humorous) {
        right_humorous->setMinLength(l);
    }
    
    if (left_humorous) {
        left_humorous->setMinLength(l);
    }
    
    if (right_arm) {
        right_arm->setMinLength(l);
    }
    
    if (left_arm) {
        left_arm->setMinLength(l);
    }
    
    if (rhip) {
        rhip->setMinLength(l);
    }
    
    if (lhip) {
        lhip->setMinLength(l);
    }
    
    if (rshoulder) {
        rshoulder->setMinLength(l);
    }
    
    if (lshoulder) {
        lshoulder->setMinLength(l);
    }
}

void trunk::setMaxLength(float l) {
    max_length = l;
    
    if (head) {
        head->setMaxLength(l);
    }
    
    if (neck) {
        neck->setMaxLength(l);
    }
    
    if (right_humorous) {
        right_humorous->setMaxLength(l);
    }
    
    if (left_humorous) {
        left_humorous->setMaxLength(l);
    }
    
    if (right_humorous) {
        right_arm->setMaxLength(l);
    }
    
    if (left_arm) {
        left_arm->setMaxLength(l);
    }
    
    if (rhip) {
        rhip->setMaxLength(l);
    }
    
    if (lhip) {
        lhip->setMaxLength(l);
    }
    
    if (rshoulder) {
        rshoulder->setMaxLength(l);
    }
    
    if (lshoulder) {
        lshoulder->setMaxLength(l);
    }
}

void trunk::setMaxSize(float s) {
    max_size = s;
    
    if (head) {
        head->setMaxSize(s);
    }

    if (neck) {
        neck->setMaxSize(s);
    }
    
    if (right_humorous) {
        right_humorous->setMaxSize(s);
    }
    
    if (left_humorous) {
        left_humorous->setMaxSize(s);
    }
    
    if (right_arm) {
        right_arm->setMaxSize(s);
    }
    
    if (left_arm) {
        left_arm->setMaxSize(s);
    }
    
    if (lhip) {
        lhip->setMaxSize(s);
    }
    
    if (rhip) {
        rhip->setMaxSize(s);
    }
    
    if (lshoulder) {
        lshoulder->setMaxSize(s);
    }
    
    if (rshoulder) {
        rshoulder->setMaxSize(s);
    }
}

void trunk::setScale(float s) {
    scale = s;
    
    if (head) {
        head->setScale(s);
    }
    
    if (neck) {
        neck->setScale(s);
    }
    
    if (right_humorous) {
        right_humorous->setScale(s);
    }
    
    if (left_humorous) {
        left_humorous->setScale(s);
    }
    if (right_arm) {
        right_arm->setScale(s);
    }
    
    if (left_arm) {
        left_arm->setScale(s);
    }
    
    if (lhip) {
        lhip->setScale(s);
    }
    
    if (rhip) {
        rhip->setScale(s);
    }
    
    if (lshoulder) {
        lshoulder->setScale(s);
    }
    
    if (rshoulder) {
        rshoulder->setScale(s);
    }
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

float trunk::getScale() {
    return scale;
}