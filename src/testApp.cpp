#include "testApp.h"
#include <cmath>

#define TWIG_MAX_SIZE 33
#define TWIG_MIN_LENGTH 20
#define TWIG_MAX_LENGTH 50
#define P_BIFURCATE 0.1
#define P_GROW 0.15
#define THICKNESS 3 

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_NOTICE);
    debugMode = true;

    kinect.setup();
    kinect.addDepthGenerator();
    kinect.addUserGenerator();
    kinect.addImageGenerator();
    kinect.setRegister(true);
    kinect.setMirror(true);

    kinect.setMaxNumUsers(1); // default is 4

    ofAddListener(kinect.userEvent, this, &testApp::userEvent);

    // Set up GUI for tweaking parameters.
    float canvasW = ofGetWidth() - 640;
    float widgetW = canvasW - OFX_UI_GLOBAL_PADDING * 2;
    gui = new ofxUICanvas(640, 0, canvasW, ofGetHeight());
    gui->setColorBack(0xaaaaaa);
    gui->addWidgetDown(new ofxUILabel("Tree Skeleton", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 0, 1, P_GROW, "GROW RATE"));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 0, 0.3, P_BIFURCATE, "BIFURCATE RATE"));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 1, 10, THICKNESS, "THICKNESS"));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 10, 60, TWIG_MIN_LENGTH, "MIN LENGTH"));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 15, 60, TWIG_MAX_LENGTH, "MAX LENGTH"));
    gui->addWidgetDown(new ofxUISlider(widgetW, 10, 10, 500, TWIG_MAX_SIZE, "MAX SIZE"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);
    
    e = new ent(P_GROW, P_BIFURCATE, THICKNESS, TWIG_MIN_LENGTH, TWIG_MAX_SIZE, TWIG_MAX_SIZE);
}

//------------------------------------------------------------
void testApp::update(){
    ofBackground(178, 178, 178);
    
    if (kinectOn) {
        kinect.update();
        if (kinect.getNumTrackedUsers() > 0 && armsRaised(kinect.getTrackedUser(0))) {
            ofLogNotice() << "Grow!";
            e->grow();
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
    ofRect(0, 0, 640, 480);

    if (kinect.getNumTrackedUsers() > 0) {
        kinect.drawSkeleton(0);
        e->draw(kinect.getTrackedUser(0));
    }

    if (debugMode) {
        ofPushMatrix();
        ofScale(0.5, 0.5);
        ofTranslate(0, 2 * ofGetHeight() - kinect.getHeight());
        kinect.drawDebug();        
        ofPopMatrix();
        
        if (kinect.getNumTrackedUsers() > 0) {
            ofSetColor(0);

            if (armsRaised(kinect.getTrackedUser(0))) {
                ofDrawBitmapString("GROW", 640 / 2 - 14, 480 / 2);
            } else {
                ofxOpenNIUser user = kinect.getTrackedUser(0);
                ofPoint pt = user.getJoint(JOINT_LEFT_SHOULDER).getProjectivePosition();
                float angle = limbAngle(user.getLimb(LIMB_LEFT_UPPER_ARM));
                ofDrawBitmapString(ofToString(angle), pt);
                                        
                angle = limbAngle(user.getLimb(LIMB_RIGHT_UPPER_ARM));
                pt = user.getJoint(JOINT_RIGHT_SHOULDER).getProjectivePosition();
                ofDrawBitmapString(ofToString(angle), pt);
            }
        }
    }
}

bool testApp::armsRaised(ofxOpenNIUser user) {
    float langle = limbAngle(user.getLimb(LIMB_LEFT_UPPER_ARM)),
        rangle = limbAngle(user.getLimb(LIMB_RIGHT_UPPER_ARM));
    
    return (langle == langle && rangle == rangle && langle > -20 && rangle < 20);
}

float testApp::limbAngle(ofxOpenNILimb limb) {
    ofPoint start = limb.getStartJoint().getProjectivePosition();
    ofPoint end = limb.getEndJoint().getProjectivePosition();
    float angle = atan((end.y - start.y) / (end.x - start.x)) * 180 / M_PI;

    return angle;
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
    
    switch (event.userStatus) {
        case USER_TRACKING_STARTED:
            break;
            
        case USER_TRACKING_STOPPED:
            e->reset();
            break;
            
        default:
            break;
    }
}

void testApp::guiEvent(ofxUIEventArgs & event) {
    string name = event.widget->getName();
    float v = ((ofxUISlider *) event.widget)->getValue();
    
    if (name == "GROW RATE") {
        e->setPGrow(v);
    } else if (name == "BIFURCATE RATE") {
        e->setPBifurcate(v);
    } else if (name == "THICKNESS") {
        e->setThicknessFactor(v);
    } else if (name == "MIN LENGTH") {
        e->setMinLength(v);
    } else if (name == "MAX LENGTH") {
        e->setMaxLength(v);
    } else if (name == "MAX SIZE") {
        e->setMaxSize(v);
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

        case 'x':
            if (kinectOn) {
                kinect.stop();
            } else {
                kinect.start();
            }
            
            kinectOn = !kinectOn;

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

