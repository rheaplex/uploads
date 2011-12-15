////////////////////////////////////////////////////////////////////////////////
//    layout.cpp - calculating the screen layout
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


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ofAppRunner.h"

#include "layout_options.h"
#include "layout.h"


////////////////////////////////////////////////////////////////////////////////
// Configuration values
////////////////////////////////////////////////////////////////////////////////

namespace
{
  // The screen (or window, or drawing area)
  
  float screen_width;
  float screen_height;
  
  // The border between the inner cells and the edge of the drawing area, 
  //  and each other
  
  float outer_border_width = 20.0;
  float inner_border_width = outer_border_width / 2.0;

  // The gap between each eeg graph (b-cells, as opposed to the face a-cell)
  // Just use inner border? Default to it...

  float b_cell_vertical_padding = outer_border_width;
  
  // Labels
  
  float large_label_size = 12;
  float small_label_size = 12;

  // These are the values from the eegs to use
  // eeg_above is exclusive

  int eeg_low = 1;
  int eeg_above = 9;

  // The bounds of the face rectangle, or "a-cell"

  ofRectangle face_bounds_rect;

  // The bounds rectangles to render each eeg trace in
  std::vector<ofRectangle> eeg_bounds_rect;
}


////////////////////////////////////////////////////////////////////////////////
// Public layout functions
////////////////////////////////////////////////////////////////////////////////

// Accessor

float label_size_small()
{
  return small_label_size;
}

// Accessor

float label_size_large()
{
  return large_label_size;
}

// Accessor for the vertical padding between b-cells

float eeg_padding_v()
{
  return b_cell_vertical_padding;
}

// Calculate the bounds of the a-cell for the face

void calculate_face_bounds()
{
  face_bounds_rect.x = outer_border_width;
  face_bounds_rect.y = outer_border_width;
  face_bounds_rect.width = ((screen_width / 3.0) * 2.0) - 
    (outer_border_width + inner_border_width);
  face_bounds_rect.height = screen_height - (outer_border_width * 2.0);
}

// Accessor

ofRectangle & face_bounds()
{
  return face_bounds_rect;
}

// We could usefully cache many of the values this calculates for speed
// But passing the count in here means we don't have to worry about setting it

void calculate_eeg_bounds(int index, int count, ofRectangle & bounds)
{ 
  float b_height = screen_height - (outer_border_width * 2.0);
  float cell_vertical_offset = b_height / count;

  bounds.x = screen_width - (screen_width / 3.0) + inner_border_width;
  bounds.y = outer_border_width + (cell_vertical_offset * index);
  bounds.width = (screen_width / 3.0) - (outer_border_width + inner_border_width);
  bounds.height = (b_height - (b_cell_vertical_padding * (count - 1))) / count;
}

// Calculate the bounds for all the b-cells

void calculate_eegs_bounds()
{
  int count = (eeg_above - eeg_low) - 1;
  for(int i = 0; i < count; i++)
    {
      ofRectangle bounds;
      calculate_eeg_bounds(i, count, bounds);
      eeg_bounds_rect.push_back(bounds);
    }
}

// Accessor. Return the b cell for the given index

ofRectangle & eeg_bounds(int index)
{
  return eeg_bounds_rect[index];
}


////////////////////////////////////////////////////////////////////////////////
// Configuration
// (Which calls some earlier functions, and so comes last)
////////////////////////////////////////////////////////////////////////////////

// Describe the layout options to Boost

void layout_add_options(po::options_description & desc)
{
  desc.add_options()
    ("outer_border", po::value<float>(), "set outer border in pixels")
    ("inner_border", po::value<float>(), "set inner border in pixels")
    ("eeg_padding", po::value<float>(), "set distance between eeg traces")
    ("large_label_size", po::value<float>(), "set large label size in pixels")
    ("small_label_size", po::value<float>(), "set small label size in pixels");
}

// Initialize the layout variables from Boost options

void layout_initialize(const po::variables_map & vm)
{
  if(vm.count("outer_border"))
    outer_border_width = vm["outer_border"].as<float>();
  if(vm.count("inner_border"))
    inner_border_width = vm["inner_border"].as<float>();
  if(vm.count("outer_border"))
    outer_border_width = vm["outer_border"].as<float>();
  if(vm.count("eeg_padding"))
    b_cell_vertical_padding = vm["eeg_padding"].as<float>();
  if(vm.count("large_label_size"))
    large_label_size = vm["large_label_size"].as<float>();
  if(vm.count("small_label_size"))
    small_label_size = vm["small_label_size"].as<float>();

  screen_width = ofGetWidth();
  screen_height = ofGetHeight();

  // Hmmm. But this makes them the same. Handle or leave?
  // Set to default value if not set by user
  if(inner_border_width == 0)
    inner_border_width = outer_border_width / 2.0;

  calculate_face_bounds();
  calculate_eegs_bounds();
}
