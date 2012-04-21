#include "testApp.h"
#include <cmath>

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
    kinect.start();
    
    // NB: Only one device can have a user generator at a time - this is a known bug in NITE due to a singleton issue
    // so it's safe to assume that the fist device to ask (ie., deviceID == 0) will have the user generator...
    
    kinect.setMaxNumUsers(1); // defualt is 4
    ofAddListener(kinect.userEvent, this, &testApp::userEvent);

    kinect.setUseMaskTextureAllUsers(true);

    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);

    image.allocate(640, 480, OF_IMAGE_GRAYSCALE);

    root = new twig(0, 30, 0);
}

//------------------------------------------------------------
void testApp::update(){
    ofBackground(178, 178, 178);
    kinect.update();

    if (kinect.isNewFrame()) {
        for (int i = 0; i < kinect.getNumTrackedUsers(); i++) {
            ofxOpenNIUser user = kinect.getTrackedUser(i);
            ofPixels pixels = user.getMaskPixels();
            
            if (pixels.getWidth() > 0 && pixels.getHeight() > 0) {
                image.setFromPixels(pixels);
                contourFinder.findContours(image);
            } else {
                ofLogNotice() << "Mask pixels:" << pixels.getWidth() << pixels.getHeight() << pixels.getImageType();
            }
        }        
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    if (debugMode) {
        float y = 2 * ofGetHeight() - kinect.getHeight();
        ofPushMatrix();
        ofScale(0.5, 0.5);
        ofTranslate(0, y);
        
        kinect.drawDepth();
        image.draw(640, 0);
        ofTranslate(640*2, 0);
        contourFinder.draw();
        
        ofPopMatrix();
        ofPushStyle();
        ofSetColor(255, 255, 255);
        
        if (kinect.getNumTrackedUsers() > 0) {
            ofxOpenNIUser user = kinect.getTrackedUser(0);
            ofPixels pix = user.getMaskPixels();
            verdana.drawString("Mask: " + ofToString(pix.getWidth()) + ", " + ofToString(pix.getHeight()) + " " + ofToString(pix.getImageType()), 650, 48);
        }
        
        verdana.drawString("Kinect: " + ofToString(kinect.getWidth()) + ", " + ofToString(kinect.getHeight()), 650, 24);
        ofPopStyle();
    }
    
    ofSetColor(255, 255, 255);
    ofRect(0, 0, 640, 480);

//    if (kinect.getNumTrackedUsers() > 0) {
//        for (int j = 0; j < blobCount; j++) {
//            int contourLength = contourFinder.blobs[j].pts.size();
//            vector<ofPoint> original;
//            vector<ofPoint> simplified;
//            
//            original.assign(contourLength, ofPoint());
//            simplified.assign(contourLength, ofPoint());
//            
//            for (int k = 0; k < contourLength; k++) {
//                original[k] = contourFinder.blobs[j].pts[k];
//            }
//            
//            contourSimplifier.simplify(original, simplified, 0.001);
//            
//            ofSetLineWidth(0.25);
//            ofSetColor(0, 0, 0);
//            ofBeginShape();
//            for (int k = 0; k < simplified.size(); k++) {
//                ofVertex(simplified[k].x, simplified[k].y);
//            }
//            ofEndShape();
//        }
//    }
}

void testApp::stickman(ofxOpenNIUser user) {
    ofPushStyle();
    ofSetHexColor(0x000000);
    
    for (int i = 0; i < user.getNumLimbs(); i++) {
        ofxOpenNILimb limb = user.getLimb((Limb) i);
        ofPoint start = limb.getStartJoint().getProjectivePosition();
        ofPoint end = limb.getEndJoint().getProjectivePosition();
        
        ofLine(start, end);
    }
    
    ofPopStyle();
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
//    kinect.stop();
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
            kinect.stop();
            break;
            
        case OF_KEY_BACKSPACE:
            // Toggle debug mode with backspace.
            debugMode = !debugMode;
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

