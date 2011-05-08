#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxArgs.h"


#include "mainApp.h"

//========================================================================
int main(int argc, char * argv[]){
  ofxArgs* args = new ofxArgs(argc, argv);

  ofAppGlutWindow window;
  ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new mainApp(args));
  delete args;
}
