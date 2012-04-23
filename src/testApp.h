#ifndef _TEST_APP
#define _TEST_APP

#include "ofxBox2d.h"
#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxUI.h"
#include "ent.h"

class testApp : public ofBaseApp {

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    void userEvent(ofxOpenNIUserEvent & event);
    void guiEvent(ofxUIEventArgs & event);

private:

    bool armsRaised(ofxOpenNIUser user);
    float limbAngle(ofxOpenNILimb limb);

    bool debugMode;
    ofTrueTypeFont verdana;
    ofxUICanvas *gui;

    ent *e;
	ofxOpenNI kinect;
    bool kinectOn;
};

#endif
