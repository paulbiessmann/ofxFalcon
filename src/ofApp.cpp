#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    //GUI
    gui.setup();
    gui.add(guiNoiseFreq.setup("guiNoiseFreq", 0.5, 0.0, 1.0));
    gui.add(guiNoiseAmplitude.setup("guiNoiseAmp", 0.5, 0.0, 1.0));
    gui.add(guiFocal.setup("guiFocal", 0.3, 0.0, 1.0));
    gui.add(guiSaturation.setup("guiSaturation", 0.0, 0.0, 1.0));

    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(true);
    
    post.createPass<DofPass>()->setEnabled(false);

    post.createPass<KaleidoscopePass>()->setEnabled(false);

    post.createPass<NoiseWarpPass>()->setEnabled(false);

    post.createPass<EdgePass>()->setEnabled(true);

    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(true);
    post.createPass<RGBShiftPass>()->setEnabled(false);
    post.createPass<LimbDarkeningPass>()->setEnabled(true);
    //    post.createPass<SSAOPass>()->setEnabled(false);
    post.createPass<ZoomBlurPass>()->setEnabled(true);

    post.createPass<ContrastPass>()->setEnabled(true);

    
    
  //  depthOfField.setup(ofGetWidth(), ofGetHeight());

    
    // Setup light
    light.setPosition(1000, 1000, 2000);
    
	ofSetBackgroundColor(0);
	ofSetBackgroundAuto(true);
    ofEnableDepthTest();
    ofDisableAlphaBlending();

	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	xMid = ofGetWidth() / 2;
	yMid = ofGetHeight() / 2;

	linesUp.resize(50);
	linesDown.resize(50);
	linesRight.resize(50);
	linesLeft.resize(50);

	for (int i = 0; i < linesUp.size(); i++) {
		linesUp[i].a = ofPoint(xMid + i, yMid);
		linesUp[i].b = ofPoint(xMid + i, yMid - 10);
		linesUp[i].c = ofPoint(xMid + i, yMid - 20);
		linesUp[i].d = ofPoint(xMid + i, yMid - 30);
	}
	for (int i = 0; i < linesDown.size(); i++) {
		linesDown[i].a = ofPoint(xMid + i, yMid);
		linesDown[i].b = ofPoint(xMid + i, yMid + 10);
		linesDown[i].c = ofPoint(xMid + i, yMid + 20);
		linesDown[i].d = ofPoint(xMid + i, yMid + 30);
	}
	for (int i = 0; i < linesRight.size(); i++) {
		linesRight[i].a = ofPoint(xMid, yMid + i);
		linesRight[i].b = ofPoint(xMid + 10, yMid + i);
		linesRight[i].c = ofPoint(xMid + 20, yMid + i);
		linesRight[i].d = ofPoint(xMid + 30, yMid + i);
	}
	for (int i = 0; i < linesLeft.size(); i++) {
		linesLeft[i].a = ofPoint(xMid, yMid + i);
		linesLeft[i].b = ofPoint(xMid - 10, yMid + i);
		linesLeft[i].c = ofPoint(xMid - 20, yMid + i);
		linesLeft[i].d = ofPoint(xMid - 30, yMid + i);

	}


}

//--------------------------------------------------------------
void ofApp::update() {
    

  // post.createPass<EdgePass>()->setAspect(ofVec2f(5, 10)); // irgendwas um 500
    //post.createPass<EdgePass>()->setSaturation(guiSaturation);
    
//    post.createPass<DofPass>()->setFocus(guiFocal);
//    post.createPass<DofPass>()->setAperture(0.001);
//    post.createPass<DofPass>()->setMaxBlur(0.001);
//
//    post.createPass<NoiseWarpPass>()->setAmplitude(guiNoiseAmplitude);
//    post.createPass<NoiseWarpPass>()->setFrequency(guiNoiseFreq);
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    gui.draw();
    
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	int width = ofGetWidth();
	int height = ofGetHeight();

   // ofEnableAlphaBlending();
	//ofSetColor(200, 200, 200, 80);
    ofSetColor(200,200,200, 100);

	float direction;
	float xPos, yPos, zPos;

	float xSpeed;
	float ySpeed;
	float zSpeed;

	relativeTimef = ofGetElapsedTimef() - timeCounter;
    
    
/** Post **/
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    light.enable();
    
    // begin scene to post process
    post.begin(cam);
    ofScale(1, -1, 1);
    ofTranslate(-width/2, -height/2);
    
    
//    depthOfField.setFocalDistance(200);
//   // depthOfField.setFocalRange();
//    depthOfField.begin();

//    cam.begin(depthOfField.getDimensions());
    
	//ofLog() << "lines size " << lines.size();
	int lSizeUp = linesUp.size();
	int lSizeDown = linesDown.size();
	int lSizeRight = linesRight.size();
	int lSizeLeft = linesLeft.size();
	
	float grow = 20;
	
	ofNoFill();
	ofSetLineWidth(2);
    ofSetCurveResolution(200);

	for (int i = 0; i < lSizeUp; i++) {
		if (linesUp[i].d.y > 50) {
			linesUp[i].b.y -= grow;
			linesUp[i].c.y -= grow;
			linesUp[i].d.y -= grow;
		}
	//	ofDrawLine(linesUp[i].a, linesUp[i].b);
		drawFalconUD(linesUp[i], i, lSizeUp);
	}
	for (int i = 0; i < lSizeDown; i++) {
		if (linesDown[i].d.y < height - 50) {
			linesDown[i].b.y += grow;
			linesDown[i].c.y += grow;
			linesDown[i].d.y += grow;
		}
	//	ofDrawLine(linesDown[i].a, linesDown[i].b);
		drawFalconUD(linesDown[i], i, lSizeDown);
	}
	for (int i = 0; i < lSizeRight; i++) {
		if (linesRight[i].d.x < width - 100) {
			linesRight[i].b.x += grow;
			linesRight[i].c.x += grow;
			linesRight[i].d.x += grow;
		}
		//ofDrawLine(linesRight[i].a, linesRight[i].b);
		drawFalconLR(linesRight[i], i, lSizeRight);
	}
	for (int i = 0; i < lSizeLeft; i++) {
		if (linesLeft[i].d.x > 100) {
			linesLeft[i].b.x -= grow;
			linesLeft[i].c.x -= grow;
			linesLeft[i].d.x -= grow;
		}
		//ofDrawLine(linesLeft[i].a, linesLeft[i].d);
		drawFalconLR(linesLeft[i], i, lSizeLeft);
	}

//    cam.end();
//    depthOfField.end();
//    depthOfField.getFbo().draw(0,0);
   // depthOfField.drawFocusAssist(0, 0);

    // end scene and draw
    post.end();
    
    
    
    // set gl state back to original
    glPopAttrib();

}
//--------------------------------------------------------------
void ofApp::drawFalconLR(Line line, int idx, int number) {

	float dir = -idx;

    
    if(bRotate){
	ofPoint aa = ofPoint(xMid, yMid + relativeTimef * 2.2);
	ofPoint bb = ofPoint(line.b.x, line.b.y - relativeTimef * 10, sin(relativeTimef * 0.1) * 20);
	ofPoint cc = ofPoint(line.c.x, line.c.y - relativeTimef * 0.5 * dir, sin(relativeTimef * 0.02) * 20 );
	ofPoint dd = ofPoint(line.d.x + sin(relativeTimef * idx * 0.01 * sin(relativeTimef * 0.1)) * 200, line.d.y - relativeTimef * 1 * dir + sin(relativeTimef * 0.08) * 100, line.d.z + relativeTimef * abs(dir) * 0.1 + sin(relativeTimef * 0.2) * 80);
	ofDrawBezier(aa.x, aa.y, aa.z, bb.x, bb.y, bb.z, cc.x, cc.y - timeCounter, cc.z, dd.x, dd.y, dd.z);
    }
    else{
       ofDrawBezier(line.a.x, line.a.y, line.a.z, line.b.x, line.b.y, line.b.z, line.c.x, line.c.y, line.c.z, line.d.x, line.d.y, line.d.z);
    }

}
//--------------------------------------------------------------
void ofApp::drawFalconUD(Line line, int idx, int number) {

	float dir = (idx - number * 0.5);

    if(bRotate){
        ofPoint aa = ofPoint(xMid, yMid + relativeTimef * 2.2, sin(relativeTimef * 1.2)*200);
        ofPoint bb = ofPoint(line.b.x, line.b.y, sin(relativeTimef * 0.2)*200);
        ofPoint cc = ofPoint(line.c.x, line.c.y - relativeTimef * 0.2 * abs(dir), sin(relativeTimef * 0.02) * 20);
        ofPoint dd ;
        dd = ofPoint(line.d.x - relativeTimef * 0.1 * dir + sin(relativeTimef * 0.01) * 20, line.d.y + sin(relativeTimef * idx * 0.005) * 100  , line.d.z - relativeTimef * dir * 0.1);
        
        ofDrawBezier(aa.x, aa.y, aa.z, bb.x, bb.y, bb.z, cc.x, cc.y - timeCounter, cc.z, dd.x, dd.y, dd.z);
    }
    else{
        ofDrawBezier(line.a.x, line.a.y, line.a.z, line.b.x, line.b.y, line.b.z, line.c.x, line.c.y, line.c.z, line.d.x, line.d.y, line.d.z);
    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case '1':
		bRotate = !bRotate;
		timeCounter = ofGetElapsedTimef();
		break;
	case 'c':
		
		pLineA.clear();
		pLineB.clear();
		drawnPoints.clear();
		xMid = ofGetWidth() / 2;
		yMid = ofGetHeight() / 2;
		break;
	case 'h':
		bHideCursor = !bHideCursor;
		if (bHideCursor) ofHideCursor();
		else ofShowCursor();
		break;

	case 'f':
		bFullscreen = !bFullscreen;
		ofSetFullscreen(bFullscreen);
		break;

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	xMid = x;
	yMid = y;

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
