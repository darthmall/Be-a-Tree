//
//  ent.cpp
//  Be A Tree
//
//  Created by Evan Sheehan on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "trunk.h"
#include <iostream>
#include <cmath>

#define WRIST_THICKNESS 4
#define ELBOW_THICKNESS (WRIST_THICKNESS * 2.618)
#define SHOULDER_THICKNESS (ELBOW_THICKNESS * 1.618)

#define RADIANS(deg) (deg * M_PI / 180)
#define DEGREES(rad) (rad * 180 / M_PI)

trunk::trunk(float p_grow,
         float p_bifurcate,
         float thickness_factor,
         float min_length,
         float max_length,
         float max_size) : p_grow(p_grow), p_bifurcate(p_bifurcate), thickness_factor(thickness_factor), min_length(min_length), max_length(max_length), max_size(max_size) {

    timestamp = ofGetElapsedTimef();
    
    twig *head = new twig(ofRandom(-5, 5), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    twig *rhand = new twig(ofRandom(-8, 8), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    twig *lhand = new twig(ofRandom(-8, 8), 5, p_grow, p_bifurcate, thickness_factor, 5, 10, 20);
    
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

    branches[XN_SKEL_LEFT_SHOULDER] = new twig(ofRandom(0, 45), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_LEFT_ELBOW] = new twig(ofRandom(10, 35), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_LEFT_HIP] = new twig(ofRandom(0, 15), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_SHOULDER] = new twig(ofRandom(-45, 0), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_ELBOW] = new twig(ofRandom(-35, -10), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
    branches[XN_SKEL_RIGHT_HIP] = new twig(ofRandom(0, 15), 5, p_grow, p_bifurcate, thickness_factor, min_length, max_length, max_size);
}

trunk::~trunk() {
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        delete (*it).second;
    }
    
    branches.clear();
}

void trunk::draw(ofxTrackedUser user, bool debug) {
    ofxLimb limb;

    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        XnSkeletonJoint joint = (*it).first;
        twig *t = (*it).second;
        
        switch (joint) {
            case XN_SKEL_LEFT_SHOULDER:
                limb = user.left_shoulder;
                break;
                
            case XN_SKEL_LEFT_ELBOW:
                limb = user.left_upper_arm;
                break;
                
            case XN_SKEL_LEFT_HAND:
                limb = user.left_lower_arm;
                break;
                
            case XN_SKEL_LEFT_HIP:
            case XN_SKEL_RIGHT_HIP:
                limb = user.hip;
                break;
                
            case XN_SKEL_RIGHT_SHOULDER:
                limb = user.right_shoulder;
                break;
                
            case XN_SKEL_RIGHT_ELBOW:
                limb = user.right_upper_arm;
                break;
                
            case XN_SKEL_RIGHT_HAND:
                limb = user.right_lower_arm;
                break;
                
            default:
                break;
        }

    
        float angle;
        XnPoint3D pt;

        if (joint == XN_SKEL_LEFT_HIP) {
            angle = limbAngle(limb.position[1], limb.position[0]);
            pt = limb.position[0];
        } else {
            angle = limbAngle(limb);
            pt = limb.position[1];
        }

        drawTrunk(user, debug);

        ofPushMatrix();
        ofTranslate(pt.X, pt.Y);
        ofRotate((angle > 0) ? angle - 90 : angle + 90);
        
        t->draw();

        ofPopMatrix();
    }
}

void trunk::grow() {
    float now = ofGetElapsedTimef();
    float diff = now - timestamp;
    
    if (diff > 0.5) {
        timestamp = now;
        
        map<XnSkeletonJoint, twig*>::iterator it;
        for (it = branches.begin(); it != branches.end(); it++) {
            (*it).second->grow();
        }
    }
}

void trunk::reset() {
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        (*it).second->clear();
    }
}

void trunk::drawTrunk(ofxTrackedUser user, bool debug) {
    ofPushStyle();

    if (debug) {
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofSetLineWidth(2);
        user.debugDraw();
    } else {
        ofFill();
        ofSetColor(35);
    }
    
    float hipW = limbLength(user.hip);
    ofPoint l_knee_strut = jointStrut(user.left_upper_leg,
                                      user.left_lower_leg, hipW * 0.618),
        l_foot_strut = jointStrut(user.left_lower_leg, hipW),
        r_knee_strut = jointStrut(user.right_upper_leg,
                                  user.right_lower_leg, hipW * 0.618),
        r_foot_strut = jointStrut(user.right_lower_leg, hipW),
        r_elbow_strut = jointStrut(user.right_upper_arm,
                                   user.right_lower_arm, ELBOW_THICKNESS),
        r_hand_strut = jointStrut(user.right_lower_arm, WRIST_THICKNESS),
        l_elbow_strut = jointStrut(user.left_upper_arm,
                                   user.left_lower_arm, ELBOW_THICKNESS),
        l_hand_strut = jointStrut(user.left_lower_arm, WRIST_THICKNESS);
    
    ofPoint pt;
    float angle, split, theta;

    ofBeginShape();
    // Left side torso
    if (user.left_shoulder.found && user.hip.found) {
        angle = limbAngle(user.left_shoulder.position[1], user.hip.position[0]);
        pt.x = SHOULDER_THICKNESS * cos(RADIANS(angle));
        pt.y = SHOULDER_THICKNESS * sin(RADIANS(angle));
        ofVertex(user.left_shoulder.position[1].X + pt.x, user.left_shoulder.position[1].Y + pt.y);
        ofVertex(user.hip.position[0].X, user.hip.position[0].Y);
    }
//
//    // Left leg
//    if (user.left_upper_leg.found && user.left_lower_leg.found) {
//        ofVertex(user.left_upper_leg.position[1].X - l_knee_strut.x,
//                 user.left_upper_leg.position[1].Y - l_knee_strut.y);
//        ofVertex(user.left_lower_leg.position[1].X - l_foot_strut.x,
//                 user.left_lower_leg.position[1].Y - l_foot_strut.y);
//        ofVertex(user.left_lower_leg.position[1].X + l_foot_strut.x,
//                 user.left_lower_leg.position[1].Y + l_foot_strut.y);
//        ofVertex(user.left_upper_leg.position[1].X + l_knee_strut.x,
//                 user.left_upper_leg.position[1].Y + l_knee_strut.y);
//    }
//    
//    // Crotch
//    if (user.hip.found) {
//        angle = RADIANS(limbAngle(user.hip));
//        ofVertex(hipW * cos(angle), hipW * sin(angle));
//    }
//    
//    // Right leg
//    if (user.right_upper_leg.found && user.right_lower_leg.found) {
//        ofVertex(user.right_upper_leg.position[1].X - r_knee_strut.x,
//                 user.right_upper_leg.position[1].Y - r_knee_strut.y);
//        ofVertex(user.right_lower_leg.position[1].X - r_foot_strut.x,
//                 user.right_lower_leg.position[1].Y - r_foot_strut.y);
//        ofVertex(user.right_lower_leg.position[1].X + r_foot_strut.x,
//                 user.right_lower_leg.position[1].Y + r_foot_strut.y);
//        ofVertex(user.right_upper_leg.position[1].X + r_knee_strut.x,
//                 user.right_upper_leg.position[1].Y + r_knee_strut.y);
//    }
    
    // Right side torso
    if (user.hip.found && user.right_shoulder.found) {
        ofVertex(user.hip.position[1].X, user.hip.position[1].Y);

        angle = RADIANS(limbAngle(user.right_shoulder.position[1], user.hip.position[1]));
        pt.x = SHOULDER_THICKNESS * cos(angle);
        pt.y = SHOULDER_THICKNESS * sin(angle);

        ofVertex(user.right_shoulder.position[1].X + pt.x, user.right_shoulder.position[1].Y + pt.y);
    }
    
    // Right arm
    if (user.right_upper_arm.found && user.right_lower_arm.found) {
        ofVertex(user.right_upper_arm.position[1].X + r_elbow_strut.x,
                 user.right_upper_arm.position[1].Y + r_elbow_strut.y);
        ofVertex(user.right_lower_arm.position[1].X + r_hand_strut.x,
                 user.right_lower_arm.position[1].Y + r_hand_strut.y);
        ofVertex(user.right_lower_arm.position[1].X - r_hand_strut.x,
                 user.right_lower_arm.position[1].Y - r_hand_strut.y);
        ofVertex(user.right_upper_arm.position[1].X - r_elbow_strut.x,
                 user.right_upper_arm.position[1].Y - r_elbow_strut.y);
    }
    
    if (user.right_shoulder.found) {
        ofVertex(user.right_shoulder.position[1].X,
                 user.right_shoulder.position[1].Y);
    }
    
    // Left arm
    if (user.left_shoulder.found) {
        ofVertex(user.left_shoulder.position[1].X,
                 user.left_shoulder.position[1].Y);
    }
    
    if (user.left_upper_arm.found && user.left_lower_arm.found) {
        ofVertex(user.left_upper_arm.position[1].X - l_elbow_strut.x,
                 user.left_upper_arm.position[1].Y - l_elbow_strut.y);
        ofVertex(user.left_lower_arm.position[1].X - l_hand_strut.x,
                 user.left_lower_arm.position[1].Y - l_hand_strut.y);
        ofVertex(user.left_lower_arm.position[1].X + l_hand_strut.x,
                 user.left_lower_arm.position[1].Y + l_hand_strut.y);
        ofVertex(user.left_upper_arm.position[1].X + l_elbow_strut.x,
                 user.left_upper_arm.position[1].Y + l_elbow_strut.y);
    }
    
    ofEndShape(true);
    ofPopStyle();
}

void trunk::drawLimb(ofxLimb limb, float dangle, int depth, bool debug) {
    ofPushStyle();
    ofFill();
    ofSetColor(0, 255, 0);

    float angle = limbAngle(limb);
    float l = limbLength(limb);
    float startW = depth / thickness_factor * 4;
    float endW = (depth + 1) / thickness_factor * 4;
    
    ofPushMatrix();
    ofTranslate(limb.position[0].X,
                limb.position[0].Y);
    ofRotate(angle + dangle);
    
    ofBeginShape();
    ofVertex(-endW / 2, 0);
    ofVertex(endW / 2, 0);
    ofVertex(startW / 2, -l);
    ofVertex(-startW / 2, -l);
    ofEndShape();
    
    ofPopMatrix();
    ofPopStyle();
}

float trunk::limbAngle(ofxLimb limb) {
    return limbAngle(limb.position[0], limb.position[1]);
}

float trunk::limbAngle(XnPoint3D start, XnPoint3D end) {
    float angle = atan((end.Y - start.Y) / (end.X - start.X)) * 180 / M_PI;
    
    // Correct for openFrameworks' wonky coordinate system (rotation of 90 degrees points straight down).
    if (end.X > start.X) {
        // Quadrant I if the end joint is higher than the start joint, otherwise quadrant IV
        return (end.Y > start.Y) ? -angle : angle;
    } else {
        // Quadrant II if the end joint is higher than the start joint, otherwise quadrant III
        return (end.Y > start.Y) ? angle - 180 : 180 - angle;
        
    }
    return (start.X > end.X) ? -angle : angle;
}

float trunk::limbLength(ofxLimb limb) {
    return sqrt(pow(limb.position[1].X - limb.position[0].X, 2) +
                pow(limb.position[1].Y - limb.position[0].Y, 2));
}

float trunk::splitAngle(ofxLimb l1, ofxLimb l2) {
    float a1 = limbAngle(l1);
    float a2 = limbAngle(l2);
    
    return ((180 - a1) + a2) / 2;
}

ofPoint trunk::jointStrut(ofxLimb first, ofxLimb second, float width) {
    ofPoint pt;
    
    float angle = limbAngle(first);
    float split = splitAngle(first, second);
    float theta = RADIANS(180 - angle - split);
    
    pt.x = abs(width * cos(theta));
    pt.y = abs(width * sin(theta));

    return pt;
}

ofPoint trunk::jointStrut(ofxLimb limb, float width) {
    ofPoint pt;
    
    float angle = limbAngle(limb);
    float theta = RADIANS(90 - angle);
    
    pt.x = abs(width * cos(theta));
    pt.y = abs(width * sin(theta));

    return pt;
}

void trunk::setPGrow(float p) {
    p_grow = p;
}

void trunk::setPBifurcate(float p) {
    p_bifurcate = p;
}

void trunk::setThicknessFactor(float factor) {
    thickness_factor = factor;
    map<XnSkeletonJoint, twig*>::iterator it;
    for (it = branches.begin(); it != branches.end(); it++) {
        (*it).second->setThicknessFactor(factor);
    }
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