#ifndef _TEST_APP
#define _TEST_APP

#include "ofxCv.h"
//#include "ofxCvContourSimplify.h"
#include "ofxOpenNI.h"
#include "ofMain.h"
#include "twig.h"

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

    void stickman(ofxOpenNIUser user);
    void userEvent(ofxOpenNIUserEvent & event);

private:
    bool debugMode;

	ofxOpenNI kinect;
    vector<XnUserID> userIds;

    ofxCv::ContourFinder contourFinder;
    ofImage image;
    int blobCount;

    ofTrueTypeFont verdana;
    twig *root;
};

#endif
