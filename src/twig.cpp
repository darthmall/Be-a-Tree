//
//  twig.cpp
//  treeTest
//
//  Created by Evan Sheehan on 4/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "twig.h"
#include "util.h"
#include <iostream>
#include <cmath>

twig::twig(float angle,
           float length,
           float p_bifurcate,
           float thickness_scale,
           float thickness_factor,
           float min_length,
           float max_length,
           float max_size) :
angle(angle),
length(length),
p_bifurcate(p_bifurcate),
scale(thickness_scale),
thickness_factor(thickness_factor),
min_length(min_length),
max_length(max_length),
max_size(max_size),
grown(false),
left(NULL),
right(NULL),
parent(NULL) {}

twig::~twig() {
    clear();
}

void twig::update() {
    if (grown) {
        if (left && right) {
            if (ofRandom(1) < 0.5) {
                left->update();
            } else {
                right->update();
            }
        } else if (left) {
            left->update();
        } else if (right) {
            right->update();
        } else if (blossoms.size() < 7) {
            ofPoint p;
            p.x = ofRandom(-5, 5);
            p.y = ofRandom(-5, 3);
            
            blossoms.push_back(p);
        }
    } else {
        grown = true;
    }

    // Calculate the new points for the contour outline of the branch.
    float base = pow(depth() + 1, scale) / thickness_factor;
    float tip = pow(depth(), scale) / thickness_factor;
    
    contour[0].x = -(base / 2);
    contour[0].y = 0;
    contour[1].x = base / 2;
    contour[1].y = 0;
    contour[2].x = tip / 2;
    contour[2].y = -length;
    contour[3].x = -tip / 2;
    contour[3].y = -length;
}

void twig::grow() {
    // Stop growth after a certain number of nodes.
    if (size() < max_size) {
        if (left) {
            left->grow();
        }
        
        if (right) {
            right->grow();
        }
        
        float p = ofRandom(1);
        
        if (left == NULL && right == NULL) {
            twig *growth = new twig(ofRandom(-10, 10), ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, max_size);
            growth->parent = this;
            
            if (growth->angle < 0) {
                left = growth;
                right = NULL;
            } else {
                left = NULL;
                right = growth;
            }            
        } else if (p < p_bifurcate && (left == NULL || right == NULL)) {
            float growth_angle = ofRandom(15, 30);
            twig *growth;
            
            // 50/50 chance to flip the sign on the angle
            if (right != NULL || ofRandom(1) < 0.5) {
                growth_angle *= -1;
            }
            
            growth = new twig(growth_angle, ofRandom(min_length, max_length), p_bifurcate, scale, thickness_factor, min_length, max_length, max_size);
            growth->parent = this;
            if (growth_angle < 0) {
                left = growth;
            } else {
                right = growth;
            }
        }
    }
}

void twig::draw() {
    if (grown) {
        ofPushMatrix();
        ofRotate(angle);

        // Draw me
        ofBeginShape();
        for (int i = 0; i < 4; i++) {
            ofVertex(contour[i]);
        }
        ofEndShape();
        
        // Draw children
        ofTranslate(0, -length);
        if (left) {
            left->draw();
        }
        
        if (right) {
            right->draw();
        }
        
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(232, 91, 135, 177);

        for (int i = 0; i < blossoms.size(); i++) {
            ofCircle(blossoms[i].x, blossoms[i].y, 4);
        }

        ofDisableAlphaBlending();
        ofPopStyle();
        ofPopMatrix();
    }
}

void twig::append(twig *transplant) {
    if (left && right) {
        // If this twig already has two children, randomly follow one looking
        // for a place to append the transplant.
        twig *follow = (ofRandom(1) < 0.5) ? left : right;
        follow->append(transplant);
    } else if (!left) {
        left = transplant;
        transplant->parent = this;
    } else {
        right = transplant;
        transplant->parent = this;
    }
}

void twig::concat(twig *transplant) {
    transplant->parent = this;
    if (left == NULL) {
        left = transplant;
    } else if (right == NULL) {
        right = transplant;
    } else {
        twig *temp = left;
        left = transplant;
        append(temp);
    }
}

twig *twig::find_node_at_depth(int d) {
    return find_node_at_depth(d, 0);
}

twig *twig::find_node_at_depth(int d, int current_depth) {
    if (d == current_depth) {
        return this;
    }
    
    twig *ret_val = NULL;
    
    if (left) {
        ret_val = left->find_node_at_depth(d, current_depth + 1);
    }
    
    if (!ret_val && right) {
        ret_val = right->find_node_at_depth(d, current_depth + 1);
    }
    
    return ret_val;
}

void twig::setGrown() {
    grown = true;
    
    if (left) {
        left->setGrown();
    }
    
    if (right) {
        right->setGrown();
    }
}

int twig::depth() {
    int d = std::max((left != NULL) ? left->depth() : 0,
                     (right != NULL) ? right->depth() : 0);
    
    return d + 1;
}

int twig::size() {
    int s = 1;
    
    if (left ) {
        s += left->size();
    }
    
    if (right) {
        s += right->size();
    }
    
    return s;
}

void twig::clear() {
    grown = false;

    if (left) {
        left->clear();
        delete left;
        left = NULL;
    }
    
    if (right) {
        right->clear();
        delete right;
        right = NULL;
    }
}

void twig::setPBifurcate(float p) {
    p_bifurcate = p;
    
    if (left) {
        left->setPBifurcate(p);
    }
    
    if (right) {
        right->setPBifurcate(p);
    }
}

float twig::getPBifurcate() {
    return p_bifurcate;
}

void twig::setThicknessFactor(float factor) {
    thickness_factor = factor;
    
    if (left) {
        left->setThicknessFactor(factor);
    }
    
    if (right) {
        right->setThicknessFactor(factor);
    }
}

float twig::getThicknessFactor() {
    return thickness_factor;
}

void twig::setMinLength(float l) {
    min_length = l;
    
    if (left) {
        left->setMinLength(l);
    }
    
    if (right) {
        right->setMinLength(l);
    }
}

float twig::getMinLength() {
    return min_length;
}

void twig::setMaxLength(float l) {
    max_length = l;
    
    if (left) {
        left->setMaxLength(l);
    }
    
    if (right) {
        right->setMaxLength(l);
    }
}

float twig::getMaxLength() {
    return max_length;
}

void twig::setMaxSize(float s) {
    max_size = s;
    
    if (left) {
        left->setMaxSize(s);
    }
    
    if (right) {
        right->setMaxSize(s);
    }
}

float twig::getMaxSize() {
    return max_size;
}

void twig::setScale(float s) {
    scale = s;
    
    if (left) {
        left->setScale(s);
    }
    
    if (right) {
        right->setScale(s);
    }
}

float twig::getScale() {
    return scale;
}