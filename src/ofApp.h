#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxDOF.h"
#include "ofxGui.h"
#include "ofxVideoRecorder.h"


class Line {
public:
	ofPoint a;
	ofPoint b;
	ofPoint c;
	ofPoint d;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void drawFalconUD(Line line, int idx, int number);
	void drawFalconLR(Line line, int idx, int number);
    void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	vector < ofPoint > drawnPoints;
	vector < Line > linesUp;
	vector < Line > linesDown;
	vector < Line > linesRight;
	vector < Line > linesLeft;


	bool    bRotate = false;
	float   timeCounter = 0;
	float relativeTimef; 

	ofPolyline pLineA;
	ofPolyline pLineB;

	ofMesh  mesh;

	int xMid;
	int yMid;

	ofEasyCam cam;

	bool    bHideCursor = false;
	bool	bFullscreen = false;
    
    // scene stuff
    ofxPostProcessing post;
    ofLight light;
    //shader
    ofxDOF depthOfField;
    
    
    // GUI ----------------------------
    
    ofxPanel gui;
    ofxSlider<float> guiNoiseFreq;
    ofxSlider<float> guiNoiseAmplitude;
    ofxSlider<float> guiFocal;
    ofxSlider<float> guiSaturation;
    
    
    
    bool bFxaa = false;
    bool bBloom = false;
    bool bDof = false;
    bool bNoise = false;
    bool bEdge = false;
    bool bTilt = false;
    bool bGod = false;
    bool bRgb = false;
    bool bZoom = false;
    bool bContrast = false;
    bool bSSAO = false;

    
    /* Recording */
    ofxVideoRecorder    vidRecorder;
    bool                bRecording;
    ofFbo               recordFbo;
    ofFbo               recordFboFlip;
    ofPixels            recordPixels;
    string              fileName;
    string              fileExt;
    float               recordedFrame;
    float               waitCounter     = 0;
    bool                bPause = false;
    bool                bEnd;
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    int fullWidth   = 3840;
    int fullHeight  = 2160;
    
    
};
