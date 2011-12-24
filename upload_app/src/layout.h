////////////////////////////////////////////////////////////////////////////////
//    layout.h - calculating the screen layout
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

#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "ofColor.h"

class ofRectangle;

extern float label_size_small();

extern float label_size_large();

extern ofRectangle & eeg_bounds(int index);
extern ofRectangle & face_bounds();

extern void background_colour(ofColor & col);
extern void foreground_colour(ofColor & col);

float eeg_line_width();
float frame_line_width();

#endif
