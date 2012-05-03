#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenNI.h"
#include "ofxUI.h"
#include "trunk.h"

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

    void guiEvent(ofxUIEventArgs & event);

private:

    bool fullscreen;
    bool paused;
    bool displayInstructions;
    float timeSinceArmsRaised;
    
    ofxHardwareDriver hardware;

    ofxOpenNIContext context;
    ofxDepthGenerator depthGenerator;
    ofxImageGenerator imageGenerator;
    ofxUserGenerator userGenerator;

    ofxCv::ContourFinder contourFinder;
    
    // For the user generator
    float filterFactor;

    map<XnUserID, trunk*> people;

    // Debug stuff
    bool debug;
    ofxUICanvas *gui;

    bool armsRaised(ofxTrackedUser user);
    
    ofTrueTypeFont titleFont;
    ofTrueTypeFont subtitleFont;
    ofImage splash;
};

#endif
