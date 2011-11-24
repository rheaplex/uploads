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

#include "ofPoint.h"
#include "ofRectangle.h"

extern float label_size_small();
extern float label_size_large();
extern void calculate_face_bounds(ofRectangle & bounds);
extern float eeg_padding_v();
extern void calculate_eeg_bounds(int index, int count, ofRectangle & bounds);

#endif
