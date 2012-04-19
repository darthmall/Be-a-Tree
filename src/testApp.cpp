#include "testApp.h"
#include <cmath>

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_NOTICE);
    debugMode = false;

    kinect.setup();
    kinect.addDepthGenerator();
    kinect.addUserGenerator();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.start();
    
    // NB: Only one device can have a user generator at a time - this is a known bug in NITE due to a singleton issue
    // so it's safe to assume that the fist device to ask (ie., deviceID == 0) will have the user generator...
    
    kinect.setMaxNumUsers(1); // defualt is 4
    ofAddListener(kinect.userEvent, this, &testApp::userEvent);

    kinect.setUseMaskTextureAllUsers(true);

    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    root = new twig(0, 30, 0);
}

//--------------------------------------------------------------
void testApp::update(){
    ofBackground(255, 255, 255);
    kinect.update();

    if (root->count() < 500) {
        root->grow(0.2, 0.001, 5);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix();

    if (debugMode) {
        kinect.drawDebug();
        ofSetColor(0, 255, 0);
        string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
        verdana.drawString(msg, 20, 506);
    }

    ofPopMatrix();
    
    for (int i = 0; i < kinect.getNumTrackedUsers(); i++) {
        ofxOpenNIUser user = kinect.getTrackedUser(i);
        user.drawSkeleton();
        
        ofxOpenNIJoint elbow = user.getJoint(JOINT_RIGHT_ELBOW);
        ofxOpenNIJoint hand = user.getJoint(JOINT_RIGHT_HAND);
        ofPoint handPos = hand.getProjectivePosition();
        ofPoint elbowPos = elbow.getProjectivePosition();
        float slope = (handPos.y - elbowPos.y) / (handPos.x - elbowPos.x);
        float angle = ofRadToDeg(atan(slope));

        root->draw(hand.getProjectivePosition().x, hand.getProjectivePosition().y, angle + 90);
    }
}


//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
    
    switch (event.userStatus) {
        case USER_TRACKING_STARTED:
            userIds.push_back(event.id);
            break;
            
        case USER_TRACKING_STOPPED:
            for (vector<XnUserID>::iterator it = userIds.begin(); it < userIds.end(); it++) {
                if ((*it) == event.id) {
                    userIds.erase(it);
                }
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    // this often does not work -> it's a known bug -> but calling it on a key press or anywhere that isnt std::aexit() works
    // press 'x' to shutdown cleanly...
    kinect.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int cloudRes = -1;
    switch (key) {
        case '1':
            cloudRes = 1;
            break;
        case '2':
            cloudRes = 2;
            break;
        case '3':
            cloudRes = 3;
            break;
        case '4':
            cloudRes = 4;
            break;
        case '5':
            cloudRes = 5;
            break;
        case 'i':
            if (kinect.isImageOn()){
                kinect.removeImageGenerator();
                kinect.addInfraGenerator();
            } else if (kinect.isInfraOn()){
                kinect.removeInfraGenerator();
                kinect.addImageGenerator();
            }
            break;
        case 'b':
            kinect.setUseBackBuffer(!kinect.getUseBackBuffer());
            break;
            
        case OF_KEY_BACKSPACE:
            // Toggle debug mode with backspace.
            debugMode = !debugMode;
            break;
            
        default:
            break;
    }
    
    kinect.setPointCloudResolutionAllUsers(cloudRes);
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

