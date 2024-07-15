#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Branch.h"

#define MOUSE_MODE 1


class App : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed( int key );
    void keyReleased( int key );
    void mouseMoved( int x, int y );
    void mouseDragged( int x, int y, int button );
    void mousePressed( int x, int y, int button );
    void mouseReleased( int x, int y, int button );
    void windowResized( int w, int h );
    void exit();
    
    void clearCanvas();
    
    ofVideoGrabber movie;
    
    ofxCvColorImage     rgb, hsb;
    ofxCvGrayscaleImage hue, sat, bri, filtered;
    ofxCvContourFinder  contours;
    
    int w,h;
    int findHue;
    int findSat;
    int findBri;
    int anchoHue;
    int anchoSat;
    int anchoBri;
    
    // Clydias
    vector<Branch*>	branches;
    ofRectangle     drawRect;
    
    bool bDrawDiag;
    bool bClearCanvas;
    bool bDrawVideo;
};

