#include "testApp.h"
#include "util.h"
#include <cmath>
#include <set>

#define TWIG_MAX_SIZE 130
#define TWIG_MIN_LENGTH 13
#define TWIG_MAX_LENGTH 25
#define P_BIFURCATE 0.33
#define THICKNESS 4.5
#define SCALE 1.5
#define GROWTH_RATE 0.1


//--------------------------------------------------------------
void testApp::setup() {
    ofSetLogLevel(OF_LOG_NOTICE);
    debug = false;
    fullscreen = false;

    // Set up and configure the Kinect
    filterFactor = 0.1f;

    // Playback from a video file for testing
    hardware.setup();
    
    context.setup();
//    context.setupUsingRecording(ofToDataPath("test2.oni"));
    depthGenerator.setup(&context);
    imageGenerator.setup(&context);
    userGenerator.setup(&context);
    userGenerator.setSmoothing(filterFactor);
    userGenerator.setUseMaskPixels(false);

    context.toggleRegisterViewport();
    context.toggleMirror();

    // Set up GUI for tweaking parameters.
    float canvas_w = ofGetWidth() - 660;
    float widget_w = canvas_w - OFX_UI_GLOBAL_WIDGET_SPACING * 2;
    gui = new ofxUICanvas(650, 10, canvas_w, 460);
    gui->setColorBack(0x5F6273);
    gui->addWidgetDown(new ofxUILabel("Be A Tree Console", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 10, 200, TWIG_MAX_SIZE, "MAX NODES"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 0.01, 1, P_BIFURCATE, "% BIFURCATE"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 5, 30, TWIG_MIN_LENGTH, "MIN LENGTH"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 5, 30, TWIG_MAX_LENGTH, "MAX LENGTH"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 0.1, 10, THICKNESS, "THICKNESS FACTOR"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 0.1, 10, SCALE, "THICKNESS SCALE"));
    gui->addWidgetDown(new ofxUISlider(widget_w, 10, 0.1, 10, GROWTH_RATE, "GROWTH RATE"));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
//    person = new trunk(P_BIFURCATE, SCALE, THICKNESS, TWIG_MIN_LENGTH, TWIG_MAX_LENGTH, TWIG_MAX_SIZE, GROWTH_RATE);
    
    contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(150);
}

//------------------------------------------------------------
void testApp::update(){
    ofBackground(255);
    
    if (!paused) {
        context.update();
        depthGenerator.update();
        imageGenerator.update();
        userGenerator.update();
        
        if (userGenerator.getNumberOfTrackedUsers() > 0) {
            ofImage mask;
            mask.setFromPixels(userGenerator.getUserPixels(), 640, 480, OF_IMAGE_GRAYSCALE);
            
            contourFinder.findContours(mask);

            set<XnUserID> found;
            for (int i = 1; i <= userGenerator.getNumberOfTrackedUsers(); i++) {
                ofxTrackedUser *user = userGenerator.getTrackedUser(i);
                
                found.insert(user->id);

                if (!people.count(user->id)) {
                    people[user->id] = new trunk(P_BIFURCATE, SCALE, THICKNESS, TWIG_MIN_LENGTH, TWIG_MAX_LENGTH, TWIG_MAX_SIZE, GROWTH_RATE);
                } else {
                    people[user->id]->update(!armsRaised(*user));
                }
            }
            
            map<XnUserID, trunk*>::iterator it;
            for (it = people.begin(); it != people.end(); it++) {
                if (!found.count((*it).first)) {
                    (*it).second->reset();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix();
    
    if (fullscreen) {
        float x = ofGetWindowHeight() / 480.f;
        ofTranslate((ofGetWindowWidth() - (640.f * x)) / 2.f, 0);
        ofScale(x, x);
    }
    
    ofSetColor(255);
    ofRect(0, 0, 640, 480);
    
    if (debug && !fullscreen) {
        ofPushMatrix();
        ofTranslate(0, 490);
        ofScale(0.5f, 0.5f);
        depthGenerator.draw(0, 0, 640, 480);
        imageGenerator.draw(640, 0, 640, 480);
        
        for (int i = 1; i <= userGenerator.getNumberOfTrackedUsers(); i++) {
            userGenerator.getTrackedUser(i)->debugDraw();
        }
        
        ofPopMatrix();
    }
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        for (int i = 1; i <= userGenerator.getNumberOfTrackedUsers(); i++) {
            ofxTrackedUser *user = userGenerator.getTrackedUser(i);
            
            if (people.count(user->id)) {
                people[user->id]->draw(*user);
            }
        }

        if (!debug) {
            ofPushStyle();
            ofFill();
            ofSetHexColor(0x292E36);
            vector<vector<cv::Point> > contours = contourFinder.getContours();
            for (int i = 0; i < contours.size(); i++) {
                vector<cv::Point> contour = contours[i];
                
                ofBeginShape();
                for (int j = 0; j < contour.size(); j++) {
                    ofVertex(contour[j].x, contour[j].y);
                }
                ofEndShape();
            }
            ofPopStyle();
        }
    }
    
    ofPopMatrix();
}

bool testApp::armsRaised(ofxTrackedUser user) {
    float langle = ofRadToDeg(limbAngle(user.left_upper_arm));
    float rangle = ofRadToDeg(limbAngle(user.right_upper_arm));
    
    return (langle == langle && rangle == rangle &&
            langle > 110  && rangle < 30);
}

void testApp::guiEvent(ofxUIEventArgs & event) {
    string name = event.widget->getName();
    ofxUISlider *slider = (ofxUISlider *) event.widget;
    float value = slider->getScaledValue();

    for (int i = 1; i < userGenerator.getNumberOfTrackedUsers(); i++) {
        ofxTrackedUser *user = userGenerator.getTrackedUser(i);
        
        if (name == "MIN LENGTH") {
            people[user->id]->setMinLength(value);
        } else if (name == "MAX LENGTH") {
            people[user->id]->setMaxLength(value);
        } else if (name == "THICKNESS SCALE") {
            people[user->id]->setScale(value);
        } else if (name == "THICKNESS FACTOR") {
            people[user->id]->setThicknessFactor(value);
        } else if (name == "% BIFURCATE") {
            people[user->id]->setPBifurcate(value);
        } else if (name == "MAX NODES") {
            people[user->id]->setMaxSize(value);
        } else if (name == "GROWTH RATE") {
            people[user->id]->growthRate = value;
        }
    }
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int cloudRes = -1;
    switch (key) {
        case 'd':
            debug = !debug;
            break;
            
        case 'f':
            fullscreen = !fullscreen;
            if (fullscreen) {
                gui->disable();
            } else {
                gui->enable();
            }
            ofSetFullscreen(fullscreen);
            break;
            
        case ' ':
            paused = !paused;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

