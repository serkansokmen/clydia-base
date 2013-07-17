#include "testApp.h"

// Init video related
int camWidth = 640;
int camHeight = 480;


//--------------------------------------------------------------
void testApp::setup(){

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
    
#ifndef MOUSE_MODE
    movie.initGrabber(w, h, true);
    
    // reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
#endif
	
	drawRect.x = 0;
	drawRect.set(0, 0, ofGetWidth(), ofGetHeight());
	
    bDrawVideo = true;
	bClearCanvas = false;
    bDrawDiag = false;
}

//--------------------------------------------------------------
void testApp::update(){
    
#ifndef MOUSE_MODE
	movie.update();
	
	if (movie.isFrameNew())
    {
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++)
        {
            if(ofInRange(hue.getPixels()[i],findHue-anchoHue,findHue+anchoHue) &&
               ofInRange(sat.getPixels()[i],findSat-anchoSat,findSat+anchoSat) &&
               ofInRange(bri.getPixels()[i],findBri-anchoBri,findBri+anchoBri))
            {
                filtered.getPixels()[i] =255;
            }
            else
            {
                filtered.getPixels()[i] =0;
            }
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 50, w*h/2, 1, false);
    }
	
	// Update tracked positions
	if (contours.nBlobs > 0)
	{
		for (int i=0; i<contours.nBlobs; i++)
		{
            ofVec2f *tPos = new ofVec2f;
            ofPoint *centroid = new ofPoint;
            
			centroid = &contours.blobs[i].centroid;
			
			tPos->x = ofMap(centroid->x, 0, w, drawRect.x, drawRect.x + drawRect.width);
			tPos->y = ofMap(centroid->y, 0, h, drawRect.y, drawRect.y + drawRect.height);
			
			Branch *branch = new Branch;
			tPos->x += ofRandom(-1, 1) * 10;
            tPos->y += ofRandom(-1, 1) * 10;
			branch->setup(*tPos, drawRect);
            //(ofRandom(0, 10) < 8) ? branch->setDrawMode(CL_BRANCH_DRAW_LEAVES) : branch->setDrawMode(CL_BRANCH_DRAW_CIRCLES);
            branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
			branches.push_back(branch);
		}
	}
#endif
    
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
void testApp::draw(){

    //ofBackgroundGradient(ofColor::gray * 0.4, ofColor::gray * 0.2, OF_GRADIENT_CIRCULAR);
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
#ifndef MOUSE_MODE
	if (bDrawVideo)
    {
        ofSetColor(255);
        //draw RGB image
        rgb.draw(0, 0);
        //draw rects for found blobs
        for (int i=0; i<contours.nBlobs; i++)
        {
            ofSetColor(255, 120);
            ofNoFill();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofRect(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, contours.blobs[i].boundingRect.width, contours.blobs[i].boundingRect.height);
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofFill();
        }
    }
#endif
    
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
void testApp::clearCanvas()
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
void testApp::keyPressed(int key){
    switch (key)
	{
		case ' ':
			bClearCanvas = true;
			break;
        case 'b':
            bDrawDiag = !bDrawDiag;
            break;
        case 'd':
            bDrawVideo = !bDrawVideo;
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
#ifdef MOUSE_MODE //1
    Branch *branch = new Branch;
    branch->setup(ofVec2f(x, y), drawRect);
    branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
    branches.push_back(branch);
#endif
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
#ifndef MOUSE_MODE
	if(x < w && y < h)
    {
        //get hue value on mouse position
        findHue = hue.getPixels()[y*w+x];
        findSat = sat.getPixels()[y*w+x];
        findBri = bri.getPixels()[y*w+x];
    }
#endif
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
    clearCanvas();
}
