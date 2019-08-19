#include "ofApp.h"

float fps = 25;

float sceneLength = 1200; //frames

//--------------------------------------------------------------
void ofApp::setup() {
    /************ Video Record *********/
    recordedFrame = 0;
    ofSetFrameRate((int) fps);
    ofSetLogLevel(OF_LOG_VERBOSE);
    fileName = "Falcon";
    fileExt = ".mov";
    vidRecorder.setVideoCodec("prores");
    vidRecorder.setVideoBitrate("2000k");
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    recordFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    recordFboFlip.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    bRecording = false;
    
    keyReleased('r');
    bEnd = false;
    
    
    //GUI
    gui.setup();
    gui.add(guiNoiseFreq.setup("guiNoiseFreq", 0.5, 0.0, 1.0));
    gui.add(guiNoiseAmplitude.setup("guiNoiseAmp", 0.5, 0.0, 1.0));
    gui.add(guiFocal.setup("guiFocal", 0.3, 0.0, 1.0));
    gui.add(guiSaturation.setup("guiSaturation", 0.0, 0.0, 1.0));

    bEdge = false;
    bZoom = false;

     bFxaa = false;
     bBloom = true;
     bDof = true;
     bNoise = false;
     bTilt = false;
     bGod = true;
     bRgb = false;
     bContrast = false;
     bSSAO = false;
    
    ofSetCoordHandedness(OF_RIGHT_HANDED);

    // Setup post-processing chain
    post.init(fullWidth, fullHeight);
   // post.createPass<DofPass>()->setEnabled(bDof);
    post.createPass<ZoomBlurPass>()->setEnabled(bZoom);
    post.createPass<NoiseWarpPass>()->setEnabled(bNoise);
    post.createPass<DofPass>()->setFocus(0.9999);
    post.createPass<FxaaPass>()->setEnabled(bFxaa);
    post.createPass<BloomPass>()->setEnabled(bBloom);
    post.createPass<GodRaysPass>()->setEnabled(bGod);
    post.createPass<SSAOPass>()->setEnabled(bSSAO);
    post.createPass<ContrastPass>()->setEnabled(bContrast);
    
    post.createPass<EdgePass>()->setEnabled(bEdge);



    
  //  depthOfField.setup(ofGetWidth(), ofGetHeight());

    
    // Setup light
    light.setPosition(1000, 1000, 2000);
    
	ofSetBackgroundColor(0);
	ofSetBackgroundAuto(true);
    ofEnableDepthTest();
    ofDisableAlphaBlending();

	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	xMid = fullWidth / 2;
	yMid = fullHeight / 2;

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
    float t =  ofGetElapsedTimef();
  //  post[2]->setAspect(ofVec2f(200, sin(t)*200)); // irgendwas um 500
    

    
//    if (ofGetFrameNum() % 60 == 0){
//        post.createPass<EdgePass>()->setEnabled(bEdge);
//        post.createPass<DofPass>()->setEnabled(bDof);
//    }

    //post.createPass<EdgePass>()->setSaturation(guiSaturation);
    
//    post.createPass<DofPass>()->setFocus(guiFocal);
//    post.createPass<DofPass>()->setAperture(0.001);
//    post.createPass<DofPass>()->setMaxBlur(0.001);
//
//    post.createPass<NoiseWarpPass>()->setAmplitude(guiNoiseAmplitude);
//    post.createPass<NoiseWarpPass>()->setFrequency(guiNoiseFreq);
    
    
    recordFbo.getTexture().readToPixels(recordPixels);
    if(bRecording){
        bool success = vidRecorder.addFrame(recordPixels);
        
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	int width = fullWidth;
	int height = fullHeight;

   // ofEnableAlphaBlending();
	//ofSetColor(200, 200, 200, 80);
    ofSetColor(255,255,255, 255);

	relativeTimef = ofGetElapsedTimef() - timeCounter;
    
    
    recordFbo.begin();
    ofClear(0,0,0,255);

    
/** Post **/
// //    copy enable part of gl state
   glPushAttrib(GL_ENABLE_BIT);
//
//    // setup gl state
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//
//

    light.enable();

    // begin scene to post process
    post.begin(cam);
    ofScale(1, -1, 1);
    ofTranslate(-width/2, -height/2);


    int lSizeUp = linesUp.size();
    int lSizeDown = linesDown.size();
    int lSizeRight = linesRight.size();
    int lSizeLeft = linesLeft.size();

    float grow = 40;

    ofNoFill();
    ofSetLineWidth(2);
    ofSetCurveResolution(200);

    for (int i = 0; i < lSizeUp; i++) {
        if (linesUp[i].d.y > 50) {
            linesUp[i].b.y -= grow;
            linesUp[i].c.y -= grow;
            linesUp[i].d.y -= grow;
        }
    //    ofDrawLine(linesUp[i].a, linesUp[i].b);
        drawFalconUD(linesUp[i], i, lSizeUp);
    }
    for (int i = 0; i < lSizeDown; i++) {
        if (linesDown[i].d.y < height - 500) {
            linesDown[i].b.y += grow;
            linesDown[i].c.y += grow;
            linesDown[i].d.y += grow;
        }
    //    ofDrawLine(linesDown[i].a, linesDown[i].b);
        drawFalconUD(linesDown[i], i, lSizeDown);
    }
    for (int i = 0; i < lSizeRight; i++) {
        if (linesRight[i].d.x < width - 1000) {
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


    // end scene and draw
    post.end();


    
    // set gl state back to original
    glPopAttrib();
    
//    ofSetColor(255);
//    gui.draw();
    
    recordFbo.end();
    
    light.disable();
    recordFbo.draw(0,0, fullWidth/4, fullHeight/4);
 
    
    if(recordedFrame - vidRecorder.getVideoQueueSize() > sceneLength + 50 && vidRecorder.getVideoQueueSize() < 5 ){
        keyReleased('r');
    }
    
    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
    }


}
//--------------------------------------------------------------
void ofApp::drawFalconLR(Line line, int idx, int number) {

	float dir = -idx;

    
    if(bRotate){
	ofPoint aa = ofPoint(xMid, yMid + relativeTimef * 2.2);
	ofPoint bb = ofPoint(line.b.x, line.b.y - relativeTimef * 10, sin(relativeTimef * 0.1) * 20);
	ofPoint cc = ofPoint(line.c.x, line.c.y - relativeTimef * 0.5 * dir, sin(relativeTimef * 0.02) * 20 );
	ofPoint dd = ofPoint(line.d.x + sin(relativeTimef * idx * 0.01 * sin(relativeTimef * 0.1)) * 200, line.d.y - relativeTimef * 1 * dir + sin(relativeTimef * 0.08) * 100, line.d.z + relativeTimef * abs(dir) * 1.1 + sin(relativeTimef * 0.2) * 80);
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
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
        case 'q':
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
            
//        case 'q':
//            bEdge = !bEdge;
//            post[0]->setEnabled(bEdge);
//            break;
//
//        case 'w':
//            bZoom = !bZoom;
//            post[1]->setEnabled(bSSAO);
//
//            break;
//
//        case 'e':
//            bDof = !bDof;
//            post[2]->setEnabled(bDof);
//            break;
//
//        case 'r':
//            bNoise = !bNoise;
//            post[3]->setEnabled(bNoise);
//            break;
//
//        case 't':
//            bFxaa = !bFxaa;
//            post[4]->setEnabled(bFxaa);
//            break;
//
//        case 'z':
//            bTilt = !bTilt;
//            post[5]->setEnabled(bTilt);
//            break;
//
//        case 'u':
//            bGod = !bGod;
//            post[6]->setEnabled(bGod);
//            break;
//
//        case 'i':
//            bRgb = !bRgb;
//            post[7]->setEnabled(bRgb);
//            break;
//
//        case 'o':
//            bBloom = !bBloom;
//            post[8]->setEnabled(bBloom);
//            break;
//
//        case 'p':
//            bContrast = !bContrast;
//            post[9]->setEnabled(bZoom);
//            break;
//
//        case '0':
//            bSSAO = !bSSAO;
//            post[10]->setEnabled(bContrast);
//            break;
            
    }
    
    
    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            //insert your path in the following row and your settings 1920x1080 60fps - the last 2 parms are zero because in my case I don't want to record audio..
            ofFilePath f;
            vidRecorder.setup( fileName+"_"+ofGetTimestampString()+".mov", recordFbo.getWidth(),recordFbo.getHeight(), (int) fps, 0, 0);
            
            //Start recording
            vidRecorder.start();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
//    xMid = x;
//    yMid = y;

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


//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
    
    
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}
