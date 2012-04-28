#include "testApp.h"
#include "util.h"
#include <cmath>

#define TWIG_MAX_SIZE 33
#define TWIG_MIN_LENGTH 20
#define TWIG_MAX_LENGTH 50
#define P_BIFURCATE 0.1
#define THICKNESS 4 


//--------------------------------------------------------------
void testApp::setup() {
    ofSetLogLevel(OF_LOG_NOTICE);
    debug = false;

    // Set up and configure the Kinect
    filterFactor = 0.1f;

    // Playback from a video file for testing
    context.setupUsingRecording(ofToDataPath("test2.oni"));
    depthGenerator.setup(&context);
    imageGenerator.setup(&context);
    userGenerator.setup(&context);
    userGenerator.setSmoothing(filterFactor);
    userGenerator.setUseMaskPixels(true);

    context.toggleRegisterViewport();
    context.toggleMirror();

    // Set up GUI for tweaking parameters.
    float canvas_w = ofGetWidth() - 640;
    float widget_w = canvas_w - OFX_UI_GLOBAL_WIDGET_SPACING * 2;
    gui = new ofxUICanvas(640, 0, canvas_w, 480);
    gui->setColorBack(0x5F6273);
    gui->addWidgetDown(new ofxUILabel("Be A Tree Console", OFX_UI_FONT_LARGE));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    tree = new trunk(P_BIFURCATE, THICKNESS, TWIG_MIN_LENGTH, TWIG_MAX_SIZE, TWIG_MAX_SIZE);
    
    contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(150);
}

//------------------------------------------------------------
void testApp::update(){
    ofBackgroundHex(0x5F6273);
    
    context.update();
    depthGenerator.update();
    imageGenerator.update();
    userGenerator.update();
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        ofImage mask;
        mask.setFromPixels(userGenerator.getUserPixels(), 640, 480, OF_IMAGE_GRAYSCALE);
        
        contourFinder.findContours(mask);

        if (armsRaised(*userGenerator.getTrackedUser(1))) {
            tree->update();            
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofxTrackedUser *user = NULL;
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        user = userGenerator.getTrackedUser(1);
    }
    
    ofSetColor(226, 225, 233);
    ofRect(0, 0, 640, 480);
    
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
            
            float langle = ofRadToDeg(angle(user->left_upper_arm.position[0].X,
                                            user->left_upper_arm.position[0].Y,
                                            user->left_upper_arm.position[1].X,
                                            user->left_upper_arm.position[1].Y,
                                            user->left_lower_arm.position[1].X,
                                            user->left_lower_arm.position[1].Y));
            float rangle = ofRadToDeg(angle(user->right_upper_arm.position[0].X,
                                            user->right_upper_arm.position[0].Y,
                                            user->right_upper_arm.position[1].X,
                                            user->right_upper_arm.position[1].Y,
                                            user->right_lower_arm.position[1].X,
                                            user->right_lower_arm.position[1].Y));
            
            ofDrawBitmapString(ofToString(langle), user->left_lower_arm.position[1].X,
                               user->left_lower_arm.position[1].Y);
            ofDrawBitmapString(ofToString(rangle), user->right_lower_arm.position[1].X,
                               user->right_lower_arm.position[1].Y);
        }
        
    }
    
    if (userGenerator.getNumberOfTrackedUsers() > 0) {
        tree->draw(*userGenerator.getTrackedUser(1));

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

