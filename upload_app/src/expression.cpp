////////////////////////////////////////////////////////////////////////////////
//    expression.cpp - load serialized kinect frame data
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

//FIXME: Zeroth frame isn't drawn!

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/regex.hpp>
#include <boost/shared_array.hpp>
#include <boost/tokenizer.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofImage.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "eeg.h"
#include "emotion.h"
#include "expression.h"
#include "layout.h"


////////////////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////////////////

// The height and width of a Kinect texture

static const int TEXTURE_WIDTH = 640;
static const int TEXTURE_HEIGHT = 480;

// If we're debugging, how many frames to load
static const unsigned int debug_frames_to_load = 10;

// The rear clipping plane distance
static float rear_clip = 1.0;

// The size of the squares for the voxel render
static float voxel_size = 10.0;

// Whether we are debugging
static bool debugging = false;

// Describe the options to Boost

void expression_add_options(po::options_description & desc){
  desc.add_options()
    ("rear_clip", po::value<float>(), "distance to rear clipping pane (metres)")
    ("voxel_size", po::value<float>(), "voxel size in pixels");
}

// Initialize the variables from Boost options

void expression_initialize(const po::variables_map & vm){
  if(vm.count("rear_clip"))
    rear_clip = vm["rear_clip"].as<float>();
  if(vm.count("voxel_size"))
    voxel_size = vm["voxel_size"].as<float>();
  // Cheat and take our own copy
  debugging = vm.count("debug");
}


////////////////////////////////////////////////////////////////////////////////
// File loading
////////////////////////////////////////////////////////////////////////////////

// Load the lines of a gzipped text file into a vector of strings

void slurp_gzipped_lines(const std::string & path,
			 std::vector<std::string> & lines){
  std::ifstream file(path);//, ios_base::in | ios_base::binary);
  if(! file) {
    throw runtime_error("Problem opening file: " + path);
  }
  boost::iostreams::filtering_stream<boost::iostreams::input> in;
  in.push(boost::iostreams::gzip_decompressor());
  in.push(file);
  std::string line;
  while(std::getline(in, line)){
    lines.push_back(line);
  }
}

// Load the values of a gzipped tsv file into an array of floats

size_t slurp_gzipped_csv_floats(boost::shared_array<float> & values,
				const std::string & path,
				int stride){
  std::vector<std::string> lines;
  slurp_gzipped_lines(path, lines);
  values = boost::shared_array<float>(new float[lines.size() * stride]);
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("\t");
  for(size_t i = 0; i < lines.size(); i++){
    tokenizer::iterator tokens = tokenizer(lines[i], sep).begin();
    int index = i * stride;
    for(int j = 0; j < stride; j++){
      values[index + j] = std::atof((*tokens).c_str());
      tokens++;
    }
  }
  return lines.size();
}


////////////////////////////////////////////////////////////////////////////////
// Frames
// Each contains the Kinect depth data and texture map for a single frame
////////////////////////////////////////////////////////////////////////////////

// A frame of Kinect data

class Frame {
public:
  Frame();
  Frame(const boost::filesystem::path & path_root, double when_base);
  ~Frame();
  void render();

private:
  void calculate_smallest_y_max(boost::shared_array<float> & xyz,
				size_t coord_count);

public:
  double when;

private:
  ofImage rgb;
  ofMesh mesh;
  float smallest_y_max;
};

// The map of emotion names to vectors of frames

std::map<std::string, std::vector<Frame> > expression_frames;

// Basic constructor. For STL containers.

Frame::Frame(){
}

// Constructor. Load the frame from the path
// And make its timestamp relative to the first frame

Frame::Frame(const boost::filesystem::path & path_root, double when_base){
  // The path is of the format /a/b/c/2346.12
  when = std::atof(path_root.filename().c_str()) - when_base;
  // Load the texture map
  rgb.loadImage(path_root.string() + ".png");
  boost::shared_array<float> xyz;
  size_t num_coords = slurp_gzipped_csv_floats(xyz,
					       path_root.string() + ".xyz",
					       3);
  boost::shared_array<float> uv;
  size_t other_num_coords = slurp_gzipped_csv_floats(uv,
						     path_root.string() + ".uv",
						     2);
  // Move this to a non-debug check
  if(num_coords != other_num_coords){
    throw runtime_error("Coord count mismatch");
  }
  // Get the closest maximal distance to the y origin above or below it
  calculate_smallest_y_max(xyz, num_coords);
  // Create the mesh
  mesh.setMode(OF_PRIMITIVE_POINTS);
  // Scale the points to window co-ordinates
  ofRectangle fb = face_bounds();
  //float scale = (fb.height / smallest_y_max) / 2.0;
  for(size_t i = 0; i < num_coords; i++){
    float * point = xyz.get() + (i * 3);
    // Z Clip here rather than during rendering
    // Note that we compare abs(x) to max y to make the mesh roughly square
    if((std::abs(point[2]) < rear_clip) || 
       (std::abs(point[1]) > smallest_y_max)){ // 
      float * tex = uv.get() + (i * 2);
      // No, don't flip the Vs
      // Flip the Vs: rgb.height is zero at this point(!), so use the constant
      mesh.addTexCoord(ofVec2f(tex[0], /*TEXTURE_HEIGHT -*/ tex[1]));
      //mesh.addVertex(ofVec3f(point[0] * scale, point[1] * scale, point[2]));
      mesh.addVertex(ofVec3f(point[0], point[1], point[2]));
    }
  }
}

// Destructor

Frame::~Frame(){
}

// Calculate the smallest maximum value above or below the y origin
// e.g. if the range of y values is -1.23..4.56, this returns 1.23

void Frame::calculate_smallest_y_max(boost::shared_array<float> & xyz,
				     size_t coord_count){
  // The lowest negative value. So min neg, but you get the idea...
  float max_neg = 0.0;
  float max_pos = 0.0;
  for(size_t i = 0; i < coord_count; i++){
    double y = xyz[(3 * i) + 1];
    if(y > max_pos){
      max_pos = y;
    }
    else if(y < max_neg){
      max_neg = y;
    }
  }
  smallest_y_max = std::min(std::fabs(max_neg), max_pos);
}

// Render the frame

void Frame::render(){
  ofRectangle fb = face_bounds();

  // Enable depth testing
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPushMatrix();

  //Clip to the face drawing area
  glEnable(GL_SCISSOR_TEST);
  glScissor(fb.x, fb.y, fb.width, fb.height);

  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  // 45 fits the clip area vertical extents nicely :-)
  // This is cheating, we should scale. :-/
  gluPerspective(45, 4/3.0, 0.0, rear_clip);

  // Set up the model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Centre the face view in the scissored region
  glTranslatef(face_gl_offset_x(), 0, 0);

  rgb.bind();
  glPointSize(voxel_size);
  mesh.drawVertices();

  // And clean up
  rgb.unbind();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
}


////////////////////////////////////////////////////////////////////////////////
// Expression loading
// An expression contains a series of Kinect frames for an emotion
////////////////////////////////////////////////////////////////////////////////

// Predicate to check whether a file path is a png file (or at least ends .png)

bool is_png_file(const boost::filesystem::directory_iterator & i){
  static boost::regex png_filter( ".*\\.png" );
  bool is = true;
  if(! boost::filesystem::is_regular_file(i->status())){
    is = false;
  }
  boost::smatch what;
  if(! boost::regex_match(i->path().filename().string(), what, png_filter)){
      is = false;
  }
  return is;
}

// Load all the frames for an emotion directory into a vector of Frame objects

void load_expression(const std::string & emotion_dir,
		     std::vector<Frame> & frames){
  std::cerr << emotion_dir << std::endl;
  boost::filesystem::directory_iterator end;
  boost::filesystem::directory_iterator i(emotion_dir);
  double when_base = -1.0;
  unsigned int loaded_count = 0;
  // Load each frame
  for(; i != end; ++i ){
    // use .pngs as the indicators of times. alphabetically they're first
    if (is_png_file(i)){
      // Strip the .png extention to get a base /path/time.stamp
      boost::filesystem::path path(i->path());
      path.replace_extension("");
      std::cerr << path.string() << std::endl;
      // We need to set the base timestamp from the first png filename
      if(when_base == -1.0){
	when_base = std::atof(path.filename().c_str());
      }
      Frame frame(path, when_base);
      frames.push_back(frame);
      loaded_count++;
      if(debugging && (loaded_count > debug_frames_to_load)) {
	break;
      }
    }
  }
}

// Load emotions from disk into a map of {emotion names : frame vectors}
// Each frame on disk has the name format "seconds.fraction.png",
// with.xyz and .uv files accompanying it

void load_expressions(const std::string & person_dir){
  std::vector<std::string>::const_iterator end;
  if(debugging) {
    end = ++(emotions.begin());
  } else {
    end = emotions.end();
  }
  for(std::vector<std::string>::const_iterator i = emotions.begin();
      i != end; ++i){
    std::vector<Frame> frames;
    load_expression(person_dir + "/" + *i, frames);
    std::sort(frames.begin(), frames.end(),
	      [](const Frame & a, const Frame & b)
	        {return a.when < b.when;});
    expression_frames[*i] = frames;
  }
}


////////////////////////////////////////////////////////////////////////////////
// Expression state updating
// Just set the current frame from the current emotion for the current time
////////////////////////////////////////////////////////////////////////////////

// The frame we are currently drawing
static Frame current_frame;

// Set the frame to be rendered from the current emotion at the current time

void update_expression(const std::string & emotion, double now){
  // Make sure the value is in range
  double now_mod = std::fmod(now, expression_frames[emotion].rbegin()->when);
  std::vector<Frame>::iterator i = expression_frames[emotion].begin();
  while(i->when < now_mod){
    ++i;
    assert(i != expression_frames[emotion].end());
  }
  current_frame = *i;
}


////////////////////////////////////////////////////////////////////////////////
// Rendering
////////////////////////////////////////////////////////////////////////////////

// Render the current frame in its bounds rectangle in the layout

void draw_expression(){
  // Frame the render
  current_frame.render();
  ofRectangle bounds = face_bounds();
  ofNoFill();
  ofRect(bounds);
}
