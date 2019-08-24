////////////////////////////////////////////////////////////////////////////////
//    main.cpp - main entry point
//    Copyright (C) 2011, 2019 Rhea Myers <rhea@myers.studio>
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

#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char * argv[]){

  ofAppGlutWindow window;
  ofSetupOpenGL(&window, 1024,768, OF_WINDOW); // <-------- setup the GL context

  ofRunApp(new ofApp(argc, argv));
}
