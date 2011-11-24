////////////////////////////////////////////////////////////////////////////////
//    drawing.cpp - plotting the data
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


// TODO: labels via ofTrueType
// TDOD: colours, particularly decay for eeg (later)?

// ENSURE: eeg data is normalized when loaded or as a preprocessing step


////////////////////////////////////////////////////////////////////////////////
// includes
////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "ofGraphics.h"
#include "ofRectangle.h"

#include "eeg.h"
#include "layout.h"


////////////////////////////////////////////////////////////////////////////////
// Constants and globals
////////////////////////////////////////////////////////////////////////////////

// These are the values from the eeg values to use
// We discard the error and use the others
// eeg_above is exclusive
int eeg_low = 1;
int eeg_above = 9;

// The bounds rectangles to render each eeg trace in
std::vector<ofRectangle> eeg_bounds;


////////////////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////////////////

void calculate_eegs_bounds()
{
  int count = eeg_above - eeg_low;
  for(int i = 0; i < count; i++)
    {
      ofRectangle bounds;
      calculate_eeg_bounds(i, count, bounds);
      eeg_bounds.push_back(bounds);
    }
}

void drawing_setup()
{
  calculate_eegs_bounds();
}


////////////////////////////////////////////////////////////////////////////////
// Drawing
////////////////////////////////////////////////////////////////////////////////

void frame_eeg(int index)
{
  ofRectangle & frame = eeg_bounds[index];
  ofNoFill();
  ofSetHexColor(0xFFFFFF);
  ofRect(frame.x, frame.y, frame.width, frame.height);
}

void draw_eeg(int index)
{
  ofRectangle & frame = eeg_bounds[index];
  float step = frame.width / (levels_display_data.size() - 1);
  float x = frame.x;
  ofNoFill();
  ofSetHexColor(0xFFFFFF);
  ofBeginShape();
  for(size_t i = 0; i < levels_display_data.size(); i++)
    {
      float y = frame.y + (frame.height * levels_display_data[i].values[index]);
      //std::cerr << "(" << x << ", " << y << ") ";
      ofVertex(x, y);
      x += step;
    }
  //std::cerr << std::endl;
  ofEndShape();
}

void draw_eegs()
{
  for(int i = eeg_low; i < eeg_above; i++)
    {
      frame_eeg(i - eeg_low);
      draw_eeg(i - eeg_low);
    }
}

void draw_face()
{
  // get current frame
  // draw
  // yes, move this out of expression
}

void draw()
{
  draw_face();
  draw_eegs();
}
