#ifndef _MAIN_APP
#define _MAIN_APP

#include "ofMain.h"
#include "ofxArgs.h"

class mainApp : public ofBaseApp{
	private:
		ofxArgs* args;

	
	public:
                mainApp(ofxArgs* args);
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		float 	counter;
		bool	bSmooth;
};

#endif
