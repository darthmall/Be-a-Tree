//
//  ent.cpp
//  Be A Tree
//
//  Created by Evan Sheehan on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ent.h"
#include <iostream>

ent::ent(float p_grow,
         float p_bifurcate,
         float thickness_factor,
         float min_length,
         float max_length,
         float max_size) : p_grow(p_grow), p_bifurcate(p_bifurcate), thickness_factor(thickness_factor), min_length(min_length), max_length(max_length), max_size(max_size) {
    
    for (Joint j = JOINT_TORSO; j < JOINT_UNKOWN; j++) {
        ofxOpenNIJoint joint = user.getJoint(j);
        if (joint.isParent()) {
            joints[j] = new twig(joint.getParent().getProjectivePosition(),
                                 joint.getProjectivePosition(),
                                 p_grow,
                                 p_bifurcate,
                                 thickness_factor,
                                 min_length,
                                 max_length,
                                 max_size);
        }
    }

    timestamp = ofGetElapsedTimef();

    
    twig *head = new twig(ofRandom(-5, 5), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    twig *rhand = new twig(ofRandom(85, 95), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    twig *lhand = new twig(ofRandom(-95, -85), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    
    while (head->size() < head->getMaxSize()) {
        head->grow();
    }
    
    while (rhand->size() < rhand->getMaxSize()) {
        rhand->grow();
    }
    
    while (lhand->size() < lhand->getMaxSize()) {
        lhand->grow();
    }
    
    branches[JOINT_HEAD] = head;
    branches[JOINT_RIGHT_HAND] = rhand;
    branches[JOINT_LEFT_HAND] = lhand;

    branches[JOINT_LEFT_SHOULDER] = new twig(ofRandom(-5, 35), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[JOINT_LEFT_ELBOW] = new twig(ofRandom(-5, 35), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[JOINT_LEFT_HIP] = new twig(ofRandom(-95, -85), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[JOINT_RIGHT_SHOULDER] = new twig(ofRandom(-35, 5), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[JOINT_RIGHT_ELBOW] = new twig(ofRandom(-35, 5), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[JOINT_RIGHT_HIP] = new twig(ofRandom(85, 95), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
}

ent::~ent() {
    map<Joint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        delete (*it).second;
    }
    
    branches.clear();
}

void ent::draw(ofxOpenNIUser user) {
    float trunkW = calculateTrunkWidth(user);
    
    ofPushStyle();
    ofFill();
    ofSetColor(136, 140, 101);

//    drawTorso(user);
//    drawLimb(user, LIMB_RIGHT_UPPER_ARM, trunkW / 3);
//    drawLimb(user, LIMB_RIGHT_LOWER_ARM, trunkW / 3);
//    drawLimb(user, LIMB_LEFT_UPPER_ARM, trunkW / 3);
//    drawLimb(user, LIMB_LEFT_LOWER_ARM, trunkW / 3);
//    drawLimb(user, LIMB_LEFT_UPPER_LEG, trunkW / 2);
//    drawLimb(user, LIMB_LEFT_LOWER_LEG, trunkW / 2);
//    drawLimb(user, LIMB_RIGHT_UPPER_LEG, trunkW / 2);
//    drawLimb(user, LIMB_RIGHT_LOWER_LEG, trunkW / 2);

    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(140, 137, 86);
    
    map<Joint, twig*>::iterator it;

    for (it = branches.begin(); it != branches.end(); it++) {
        ofxOpenNIJoint joint = user.getJoint((*it).first);
        ofPoint pt = joint.getProjectivePosition();
        float angle = limbAngle(joint, joint.getParent());
        
        (*it).second->draw(pt.x, pt.y, angle);
    }

    ofPopStyle();
}

void ent::grow() {
    float now = ofGetElapsedTimef();
    float diff = now - timestamp;
    
    ofLogNotice() << "Grow:" << ofToString(timestamp) << "-" << ofToString(now) << "=" << ofToString(diff);
    
    if (diff > 0.5) {
        timestamp = now;
        
        map<Joint, twig*>::iterator it;
        for (it = branches.begin(); it != branches.end(); it++) {
            (*it).second->grow();
        }
    }
}

void ent::reset() {
    map<Joint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        (*it).second->clear();
    }
}

void ent::drawTorso(ofxOpenNIUser user) {
    ofPoint lhip = user.getJoint(JOINT_LEFT_HIP).getProjectivePosition(),
        rhip = user.getJoint(JOINT_RIGHT_HIP).getProjectivePosition(),
        lshoulder = user.getJoint(JOINT_LEFT_SHOULDER).getProjectivePosition(),
        rshoulder = user.getJoint(JOINT_RIGHT_SHOULDER).getProjectivePosition();


    ofBeginShape();
    ofVertex(lhip.x, lhip.y);
    ofVertex(rhip.x, rhip.y);
    ofVertex(rhip.x, rshoulder.y);
    ofVertex(lhip.x, lshoulder.y);
    ofEndShape();
}

void ent::drawLimb(ofxOpenNIUser user, Limb limbType, float w) {
    ofxOpenNILimb limb = user.getLimb(limbType);
    ofPoint start = limb.getStartJoint().getProjectivePosition(),
        end = limb.getEndJoint().getProjectivePosition();
    
    ofBeginShape();
    ofVertex(start.x, start.y);
    ofVertex(end.x, end.y);
    ofVertex(end.x + w, end.y);
    ofVertex(start.x + w, start.y);
    ofEndShape();
}

void ent::growTips(vector<twig> & appendage) {
    for (int i = 0; i < ofRandom(3, 6); i++) {
        twig *t = new twig(ofRandom(-5, 5),
                           ofRandom(min_length, max_length),
                           1, 0, thickness_factor,
                           min_length, max_length, max_size);
        for (int j = 0; j < ofRandom(3, 8); j++) {
            t->grow();
        }        
//        appendage.push_back(t);
    }
}

float ent::calculateTrunkWidth(ofxOpenNIUser user) {
    ofPoint rhip = user.getJoint(JOINT_RIGHT_HIP).getProjectivePosition();
    ofPoint lhip = user.getJoint(JOINT_LEFT_HIP).getProjectivePosition();
    
    return sqrt(pow(rhip.x - lhip.x, 2) + pow(rhip.y - lhip.y, 2));
}

float ent::limbAngle(ofxOpenNILimb limb) {
    return limbAngle(limb.getStartJoint(), limb.getEndJoint());
}

float ent::limbAngle(ofxOpenNIJoint start, ofxOpenNIJoint end) {
    return limbAngle(start.getProjectivePosition(), end.getProjectivePosition());    
}

float ent::limbAngle(ofPoint start, ofPoint end) {
    float angle = atan((end.y - start.y) / (end.x - start.x)) * 180 / M_PI;
    return angle;
}

void ent::setPGrow(float p) {
    p_grow = p;
}

void ent::setPBifurcate(float p) {
    p_bifurcate = p;
}

void ent::setThicknessFactor(float factor) {
    thickness_factor = factor;
}

void ent::setMinLength(float l) {
    min_length = l;
}

void ent::setMaxLength(float l) {
    max_length = l;
}

void ent::setMaxSize(float s) {
    max_size = s;
}

float ent::getPGrow() {
    return p_grow;
}

float ent::getPBifurcate() {
    return p_bifurcate;
}

float ent::getThicknessFactor() {
    return thickness_factor;
}

float ent::getMinLength() {
    return min_length;
}

float ent::getMaxLength() {
    return max_length;
}

float ent::getMaxSize() {
    return max_size;
}