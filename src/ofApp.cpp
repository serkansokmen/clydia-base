#include "ofApp.h"

// Init video related
int camWidth = 640;
int camHeight = 480;


//--------------------------------------------------------------
void ofApp::setup(){

    //------------------------------------------------
	// Main setup
	//------------------------------------------------
	ofSetBackgroundAuto(false);
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(150);
    ofSetColor(255, 255);
	ofSetVerticalSync(true);
	
	w = 320;
    h = 240;
    
    anchoHue = anchoBri = anchoSat = 25;
    findHue = 176;
    findSat = 97;
    findBri = 180;
        
    // reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
	
	drawRect.x = 0;
	drawRect.set(0, 0, ofGetWidth(), ofGetHeight());
	
    
	bClearCanvas = false;
    bDrawDiag = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    //--------------------------------------------------------------
	// update clydias
	//--------------------------------------------------------------
	for (int i=0; i<branches.size(); i++)
	{
		if (branches[i]->getIsAlive())
        {
            branches[i]->update();
        }
		else
        {
            delete branches[i];
            branches[i] = 0;
            branches.erase(branches.begin()+i);
        }
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0, 0, 0, 10);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255);
	
	if (bClearCanvas)
	{
		clearCanvas();
		bClearCanvas = false;
	}
	
    // draw branches
	for (int i=0; i<branches.size(); i++)
    {
        branches[i]->draw();
        if (bDrawDiag)
        {
            int age = branches[i]->getAge();
            ofSetColor(255, 255);
            ofDrawBitmapString("Age: " + ofToString(age), branches[i]->getPosition());
        }
    }
    
    if (bDrawDiag)
    {
        // Draw diagnostic info
        int xPad = 20;
        int yPad = 20;
        
        ofSetColor(0, 0, 0);
        ofRect(xPad-5, ofGetHeight()-yPad*4, 250, yPad*4);
        ofSetColor(255);
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), xPad, ofGetHeight() - yPad*3);
        ofDrawBitmapString("Branch count: " + ofToString(branches.size()), xPad, ofGetHeight() - yPad*1);
        
        ofColor trackColor;
        trackColor.setHue(findHue);
        trackColor.setSaturation(findSat);
        trackColor.setBrightness(findBri);
        ofSetColor(trackColor);
        ofDrawBitmapString("HUE: " + ofToString(findHue) + ", SAT: " + ofToString(findSat) + ", BRI: " + ofToString(findBri), xPad, ofGetHeight() - yPad*2);
    }
}

//--------------------------------------------------------------
void ofApp::clearCanvas()
{
    for (int i=0;i<branches.size();i++)
    {
        delete branches[i];
        branches[i] = 0;
    }
	branches.clear();
	
	ofBackground(0);
	ofSetColor(0, 255);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
	{
		case ' ':
			bClearCanvas = true;
			break;
        case 'b':
            bDrawDiag = !bDrawDiag;
            break;
        default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    Branch *branch = new Branch;
    branch->setup(ofVec2f(x, y), drawRect);
    branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
    branches.push_back(branch);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(x < w && y < h)
    {
        //get hue value on mouse position
        findHue = hue.getPixels()[y*w+x];
        findSat = sat.getPixels()[y*w+x];
        findBri = bri.getPixels()[y*w+x];
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void ofApp::exit(){
    clearCanvas();
}
