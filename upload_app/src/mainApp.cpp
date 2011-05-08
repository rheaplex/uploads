#include "twitterStreaming.h"

#include "mainApp.h"


//--------------------------------------------------------------
void mainApp::setup(){
	ofSetWindowTitle("upload");
	ofSetFrameRate(60);

	start_twitter_search("");
}

//--------------------------------------------------------------
void mainApp::update(){
  static int count = 0;
  if (count = 100){
    reset_twitter_emotion_map();
    count = 0;
  }
}

//--------------------------------------------------------------
void mainApp::draw(){
  ofSetColor(0x000000);
  ofDrawBitmapString(current_emotion.c_str(), 100, 100);
}


//--------------------------------------------------------------
void mainApp::keyPressed(int key){
}

//--------------------------------------------------------------
void mainApp::keyReleased(int key){
}

//--------------------------------------------------------------
void mainApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void mainApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void mainApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void mainApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void mainApp::windowResized(int w, int h){
}
