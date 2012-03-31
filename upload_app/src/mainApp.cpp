////////////////////////////////////////////////////////////////////////////////
//    mainApp.cpp - main OpenFrameworks application
//    Copyright (C) 2011  Rob Myers <rob@robmyers.org>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <sys/time.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "eeg.h"
#include "emotion.h"
#include "expression.h"
#include "layout.h"
#include "layoutOptions.h"
#include "twitterStreaming.h"

#include "mainApp.h"

//--------------------------------------------------------------
mainApp::mainApp(int argc, char * argv[]):
  previousEmotionReset(ofGetElapsedTimef()){
  // Set up the arguments
  po::options_description desc;
  layout_add_options(desc);
  twitter_add_options(desc);
  expression_add_options(desc);
  desc.add_options()
    ("data", po::value<std::string>(),
     "the ABSOLUTE path to the data directory");

  // Parse the arguments
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // Make sure we have the data path
  if(vm.count("data") && (vm["data"].as<std::string>()[0] == '/')){
    this->data_path = vm["data"].as<std::string>();
  }else{
    throw runtime_error("Please specify --data /absolute/path/to/data/dir");
  }

  layout_initialize(vm, ofGetScreenWidth(), ofGetScreenHeight());
  twitter_initialize(vm);
  expression_initialize(vm);
}


//--------------------------------------------------------------
void mainApp::setup(){
  // Load data from the specified
  load_emotions(this->data_path);
  load_expressions(this->data_path);

  // Set the window properties
  ofSetWindowTitle("upload");
  ofSetFrameRate(60);
  ofSetFullscreen(true);
  ofHideCursor();

  // Set the background colour
  ofColor col;
  background_colour(col);
  ofBackground(col.r, col.g, col.b);

  // Start the twitter search thread to drive the emotion state
  start_twitter_search();
} 


//--------------------------------------------------------------
void mainApp::update(){
  // Get the current time
  now = ofGetElapsedTimef();

  // Find out what the current dominant emotion is
  current_twitter_emotion(emotion);
  // If we haven't got any streaming results yet the emotion will be empty
  // So try again next time
  if(emotion == ""){
    return;
  }

  // Reset the emotion map every so often
  if((now - previousEmotionReset) >= 10){
    reset_twitter_emotion_map();
    previousEmotionReset = now;
  }

  // Update the eeg and expression state based on the current emotion and time
  update_eegs(emotion, now);
  update_expression(emotion, now);
}


//--------------------------------------------------------------
void mainApp::draw(){
  // If we haven't got any streaming results yet the emotion will be empty
  // and the state won't be configured
  // So try again next time
  if(emotion == ""){
    return;
  }

  draw_eegs();
  draw_expression();
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
