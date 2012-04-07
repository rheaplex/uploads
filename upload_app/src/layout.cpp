////////////////////////////////////////////////////////////////////////////////
//    layout.cpp - calculating the screen layout
//    Copyright (C) 2011, 2012  Rob Myers <rob@robmyers.org>
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
#include <sstream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ofAppRunner.h"

#include "layoutOptions.h"
#include "layout.h"


////////////////////////////////////////////////////////////////////////////////
// Configuration values
////////////////////////////////////////////////////////////////////////////////

namespace{
  // The screen (or window, or drawing area)
  float screen_width;
  float screen_height;
  
  // How many horizontal cells to divide the screen into
  unsigned int h_cells_count = 3;

  // How many horizontal cells to devote to the face area / a-cell
  unsigned int h_cells_face = 2;

  // And to the eeg area / b-cells
  unsigned int h_cells_eegs = h_cells_count - h_cells_face;

  // The border between the inner cells and the edge of the drawing area, 
  //  and each other
  float outer_border_width = 20.0;
  float inner_border_width = 18.0;

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

  // Colours
  ofColor background;
  ofColor foreground;

  // Line widths
  float line_width_eeg = 2.0;
  float line_width_frame = 2.0;
}


////////////////////////////////////////////////////////////////////////////////
// Public layout functions
////////////////////////////////////////////////////////////////////////////////

// Accessors

float label_size_small(){
  return small_label_size;
}

float label_size_large(){
  return large_label_size;
}

float eeg_padding_v(){
  return b_cell_vertical_padding;
}

void background_colour(ofColor & col){
  col = background;
}

void foreground_colour(ofColor & col){
  col = foreground;
}

float eeg_line_width(){
  return line_width_eeg;
}

float frame_line_width(){
  return line_width_frame;
}

ofRectangle & face_bounds(){
  return face_bounds_rect;
}

ofRectangle & eeg_bounds(int index){
  return eeg_bounds_rect[index];
}

float face_gl_offset_x() {
  // No idea :-(
  return -0.100;
}


////////////////////////////////////////////////////////////////////////////////
// Configuration
// Get values from command-line options
// And calculate layout
////////////////////////////////////////////////////////////////////////////////

// Calculate the bounds of the a-cell for the face

void calculate_face_bounds(){
  face_bounds_rect.x = outer_border_width;
  face_bounds_rect.y = outer_border_width;
  face_bounds_rect.width =
    ((screen_width / (float)h_cells_count) * (float)h_cells_face) - 
    (inner_border_width);
  face_bounds_rect.height = screen_height - (outer_border_width * 2.0);
  std::cerr << screen_height << std::endl;
  std::cerr << face_bounds_rect.height << std::endl;
}

// We could usefully cache many of the values this calculates for speed
// But passing the count in here means we don't have to worry about setting it

void calculate_eeg_bounds(int index, int count, ofRectangle & bounds){
  float b_width = (screen_width / (float)h_cells_count) * ((float)h_cells_eegs);
  float b_height = screen_height - (outer_border_width * 2.0);
  float cell_vertical_offset = b_height / count;

  bounds.x = screen_width + inner_border_width - b_width;
  bounds.y = outer_border_width + (cell_vertical_offset * index);
  bounds.width = b_width - (outer_border_width + inner_border_width);
  bounds.height = cell_vertical_offset - inner_border_width;
}

// Calculate the bounds for all the b-cells

void calculate_eegs_bounds(){
  eeg_bounds_rect.clear();
  int count = (eeg_above - eeg_low) - 1;
  for(int i = 0; i < count; i++){
      ofRectangle bounds;
      calculate_eeg_bounds(i, count, bounds);
      eeg_bounds_rect.push_back(bounds);
    }
}

// Parse a colour from a string

ofColor str2col(const std::string & colstr){
  //FIXME: Should raise exception to be handled by requestor
  if(colstr.size() != 6){
    std::stringstream str;
    str << "Colour: " << colstr << 
      " is the wrong length. Should be 6 hex digits";
    throw str.str();
  }
  ofColor col;
  //FIXME: handle out-of-range values
  col.r = strtol(colstr.substr(0, 2).c_str(), NULL, 16);
  col.g = strtol(colstr.substr(2, 2).c_str(), NULL, 16);
  col.b = strtol(colstr.substr(4, 2).c_str(), NULL, 16);
  return col;
}

// Describe the layout options to Boost

void layout_add_options(po::options_description & desc){
  desc.add_options()
    ("columns", po::value<unsigned int>(),
     "how many columns to divide the screen into for layout")
    ("face_columns", po::value<unsigned int>(),
     "how many columns to devote to the face/expresion render")
    ("outer_border", po::value<float>(), "set outer border in pixels")
    ("inner_border", po::value<float>(), "set inner border in pixels")
    ("eeg_padding", po::value<float>(), "set distance between eeg traces")
    ("large_label_size", po::value<float>(), "set large label size in pixels")
    ("small_label_size", po::value<float>(), "set small label size in pixels")
    ("frame_line_width", po::value<float>(), "set border line width in pixels")
    ("eeg_line_width", po::value<float>(), "set eeg trace line width in pixels")
    ("background", po::value<std::string>(),
     "the background colour in RGB hex, e.g. 333333")
    ("foreground", po::value<std::string>(),
     "the foreground colour in RGB hex, e.g. FFFFFF");
}

// Initialize the layout variables from Boost options

void layout_initialize(const po::variables_map & vm, float width, float height){
  // Drawing area size
  screen_width = width;
  screen_height = height;

  if(vm.count("columns"))
    h_cells_count = vm["columns"].as<unsigned int>();
  if(vm.count("face_columns"))
    h_cells_face = vm["face_columns"].as<unsigned int>();
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
  if(vm.count("frame_line_width"))
    line_width_frame = vm["frame_line_width"].as<float>();
  if(vm.count("eeg_line_width"))
    line_width_eeg = vm["eeg_line_width"].as<float>();
  if(vm.count("foreground"))
    foreground = str2col(vm["foreground"].as<std::string>());
  else
    foreground = str2col("ffffff");
  if(vm.count("background"))
    background = str2col(vm["background"].as<std::string>());
  else
    background = str2col("333333");

  // Hmmm. But this makes them the same. Handle or leave?
  // Set to default value if not set by user
  if(inner_border_width == 0)
    inner_border_width = outer_border_width / 2.0;

  if (h_cells_face >= h_cells_count){
    throw "Face cell count >= total cell count. Must be at least one less.";
  }
  h_cells_eegs = h_cells_count - h_cells_face;

  calculate_eegs_bounds();
  calculate_face_bounds();
}
