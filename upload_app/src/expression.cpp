////////////////////////////////////////////////////////////////////////////////
//    expression.cpp - load serialized kinect frame data
//    Copyright (C) 2011, 2012, 2019 Rhea Myers <rhea@myers.studio>
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
#include "ofMesh.h"

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

// File extensions
static const std::string xyz_extension = ".xyz.gz";
static const std::string uv_extension = ".uv.gz";
static const std::string xyz_bin_extension = ".xyz.bin";
static const std::string uv_bin_extension = ".uv.bin";


// The clipping plane distances
static float near_clip = 0.4;
static float far_clip = 1.4;

// The size of the squares for the voxel render
static float voxel_size = 10.0;

// The scale to apply to the voxels
static float voxel_scale = 1.5;

// Whether we are debugging
static bool debugging = false;

// Describe the options to Boost

void expression_add_options(po::options_description & desc){
  desc.add_options()
    ("near_clip", po::value<float>(), "distance to near clipping pane (metres)")
    ("far_clip", po::value<float>(), "distance to far clipping pane (metres)")
    ("voxel_size", po::value<float>(), "voxel size in pixels")
    ("voxel_scale", po::value<float>(), "how much to scale voxel space");
}

// Initialize the variables from Boost options

void expression_initialize(const po::variables_map & vm){
  if(vm.count("near_clip"))
    near_clip = vm["near_clip"].as<float>();
  if(vm.count("far_clip"))
    far_clip = vm["far_clip"].as<float>();
  if(vm.count("voxel_size"))
    voxel_size = vm["voxel_size"].as<float>();
  // Cheat and take our own copy
  debugging = vm.count("debug");
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

public:
  double when;

private:
  ofImage rgb;
  ofMesh mesh;
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
  mesh.setMode(OF_PRIMITIVE_POINTS);
  // Scale the points to window co-ordinates
  ofRectangle fb = face_bounds();
  // Load the texture map
  rgb.load(path_root.string() + ".png");
  // Load the point and uv co-ords
  std::ifstream xyz(path_root.string() + xyz_bin_extension, std::ios::binary);
  std::ifstream uv(path_root.string() + uv_bin_extension, std::ios::binary);
  // We don't know *exactly* how many coords there will be, so just read until
  // we run out. If there's a mismatch between xyz and uv counts, this will
  // raise an exception.
  while(!xyz.eof()) {
    float point[3];
    xyz.read(reinterpret_cast<char*>(&point), sizeof(float) * 3);
    float tex[2];
    uv.read(reinterpret_cast<char*>(&tex), sizeof(float) * 2);
    mesh.addVertex(ofVec3f(point[0], point[1], point[2]));
    mesh.addTexCoord(ofVec2f(tex[0], tex[1]));
    mesh.addColor(rgb.getColor(tex[0], tex[1]));
  }
  xyz.close();
  uv.close();
}

// Destructor

Frame::~Frame(){
}

// Render the frame

void Frame::render(){
  // Save current viewport
  int old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);

  // Set new viewport
  ofRectangle fb = face_bounds();
  glViewport(fb.x, fb.y, fb.width, fb.height);

  // Enable depth testing
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glPushMatrix();

  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(60, 4/3.0, near_clip, far_clip);

  // Set up the model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(voxel_scale, voxel_scale, 1.0);

  // Draw!
  rgb.bind();
  glPointSize(voxel_size);
  mesh.drawVertices();

  // And clean up
  rgb.unbind();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glDisable(GL_DEPTH_TEST);
  //glDisable(GL_SCISSOR_TEST);
  glViewport(old_viewport[0], old_viewport[1], old_viewport[2],
             old_viewport[3]);
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
  ofDrawRectangle(bounds);
}
