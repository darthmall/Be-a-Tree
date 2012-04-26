#include "testApp.h"
#include "util.h"
#include <cmath>

#define TWIG_MAX_SIZE 33
#define TWIG_MIN_LENGTH 20
#define TWIG_MAX_LENGTH 50
#define P_BIFURCATE 0.1
#define P_GROW 0.15
#define THICKNESS 4 

//--------------------------------------------------------------
void testApp::setup() {
    ofSetLogLevel(OF_LOG_NOTICE);
    debug = false;

    // Set up and configure the Kinect
    filterFactor = 0.1f;

    // Playback from a video file for testing
    context.setupUsingRecording(ofToDataPath("test.oni"));
    depthGenerator.setup(&context);
    imageGenerator.setup(&context);
    userGenerator.setup(&context);
    userGenerator.setSmoothing(filterFactor);

    context.toggleRegisterViewport();
    context.toggleMirror();

    // Set up GUI for tweaking parameters.
    float canvas_w = ofGetWidth() - 640;
    float widget_w = canvas_w - OFX_UI_GLOBAL_WIDGET_SPACING * 2;
    gui = new ofxUICanvas(640, 0, canvas_w, ofGetHeight());
    gui->setColorBack(0x5F6273);
    gui->addWidgetDown(new ofxUILabel("Be A Tree Console", OFX_UI_FONT_LARGE));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    tree = new trunk(P_GROW, P_BIFURCATE, THICKNESS, TWIG_MIN_LENGTH, TWIG_MAX_SIZE, TWIG_MAX_SIZE);
}

//------------------------------------------------------------
void testApp::update(){
    ofBackgroundHex(0x5F6273);
    
    context.update();
    depthGenerator.update();
    imageGenerator.update();
    userGenerator.update();
    
    if (userGenerator.getNumberOfTrackedUsers() > 0 &&
        armsRaised(*userGenerator.getTrackedUser(1))) {
        tree->grow();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(226, 225, 233);
    ofRect(0, 0, 640, 480);

    ofxTrackedUser *user = NULL;
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        user = userGenerator.getTrackedUser(1);
    }

    if (debug) {
        ofPushMatrix();
        ofTranslate(0, 490);
        ofScale(0.5f, 0.5f);
        depthGenerator.draw(0, 0, 640, 480);
        imageGenerator.draw(640, 0, 640, 480);
        
        if (user) {
            user->debugDraw();
        }

        ofPopMatrix();
        
        if(user) {
            user->debugDraw();
        }
    }
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        tree->draw(*userGenerator.getTrackedUser(1));
    }
}

bool testApp::armsRaised(ofxTrackedUser user) {
    float langle = ofRadToDeg(limbAngle(user.left_upper_arm));
    float rangle = ofRadToDeg(limbAngle(user.right_upper_arm));
    
    return (langle == langle && rangle == rangle &&
            langle > -20  && rangle < 20);
}

void testApp::guiEvent(ofxUIEventArgs & event) {
    string name = event.widget->getName();
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

