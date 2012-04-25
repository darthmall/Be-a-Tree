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
    
    branches[XN_SKEL_HEAD] = head;
    branches[XN_SKEL_RIGHT_HAND] = rhand;
    branches[XN_SKEL_LEFT_HAND] = lhand;

    branches[XN_SKEL_LEFT_SHOULDER] = new twig(ofRandom(-5, 35), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_LEFT_ELBOW] = new twig(ofRandom(-5, 35), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_LEFT_HIP] = new twig(ofRandom(-95, -85), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_SHOULDER] = new twig(ofRandom(-35, 5), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_ELBOW] = new twig(ofRandom(-35, 5), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_HIP] = new twig(ofRandom(85, 95), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
}

trunk::~trunk() {
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        delete (*it).second;
    }
    
    branches.clear();
}

void trunk::draw(ofxTrackedUser user) {

}

void trunk::grow() {
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        (*it).second->grow();
    }
}

void trunk::reset() {
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        (*it).second->clear();
    }
}

void trunk::drawTrunk(ofxTrackedUser user) {

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