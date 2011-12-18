////////////////////////////////////////////////////////////////////////////////
//    mainApp.h - main OpenFrameworks application
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

#ifndef _MAIN_APP_H_
#define _MAIN_APP_H_

#include <string>

#include "ofMain.h"

class mainApp : public ofBaseApp {  
 public:
  mainApp(int argc, char * argv[]);
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
  
  float counter;
  bool	bSmooth;
  
 private:
  std::string data_path;
  std::string emotion;
  double      now;
  float       previousEmotionReset;
};

#endif
