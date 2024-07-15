#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1440, 900, OF_FULLSCREEN);			// <-------- setup the GL context
    
    // Set anti-aliasing
    window.setGlutDisplayString("rgba double samples>=2 depth");
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new App());

}
