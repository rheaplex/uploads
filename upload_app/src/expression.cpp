////////////////////////////////////////////////////////////////////////////////
//    expression.cpp - load serialized kinect frame data
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

// How the voxel render rotation drifts over time
// In degrees
static float rotation_drift = 0.5f;
static float max_rotation_drift = 36.0f;
static float rotation_update_frequency = 0.2;

// The rear clipping plane distance in metres
static float rear_clip = 3.0;

// The size of the squares for the voxel render
static float voxel_size = 5.0;

// Describe the options to Boost

void expression_add_options(po::options_description & desc){
  desc.add_options()
    ("rear_clip", po::value<float>(), "distance to rear clipping pane (metres)")
    ("rotation_drift", po::value<float>(),
     "how much (+/-) to update rotation each time")
    ("rotation_max", po::value<float>(),
     "the maximum (+/-) rotation in degrees")
    ("rotation_time", po::value<float>(),
     "how often to update rotation, in seconds")
    ("voxel_size", po::value<float>(), "voxel size in pixels");
}

// Initialize the variables from Boost options

void expression_initialize(const po::variables_map & vm){
  if(vm.count("rear_clip"))
    rear_clip = vm["rear_clip"].as<float>();
  if(vm.count("rotation_drift"))
    rotation_drift = vm["rear_clip"].as<float>();
  if(vm.count("rotation_max"))
    max_rotation_drift = vm["rotation_max"].as<float>();
  if(vm.count("rear_clip"))
    rotation_update_frequency = vm["roration_time"].as<float>();
  if(vm.count("voxel_size"))
    voxel_size = vm["voxel_size"].as<float>();
}


////////////////////////////////////////////////////////////////////////////////
// File loading
////////////////////////////////////////////////////////////////////////////////

// Load the lines of a gzipped text file into a vector of strings

void slurp_gzipped_lines(const std::string & path,
			 std::vector<std::string> & lines){
  std::ifstream file(path);//, ios_base::in | ios_base::binary);
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
  void render(float rotation[3]);

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
    throw "Coord count mismatch";
  }
  // Get the closest maximal distance to the y origin above or below it
  calculate_smallest_y_max(xyz, num_coords);
  // Create the mesh
  mesh.setMode(OF_PRIMITIVE_POINTS);
  // Scale the points to window co-ordinates
  ofRectangle fb = face_bounds();
  float scale = (fb.height / smallest_y_max) / 2.0;
  for(size_t i = 0; i < num_coords; i++){
    float * point = xyz.get() + (i * 3);
    // Z Clip here rather than during rendering
    // Note that we compare abs(x) to max y to make the mesh roughly square
    if((point[2] < rear_clip) || (std::abs(point[1]) > smallest_y_max)){ // 
      float * tex = uv.get() + (i * 2);
      // Flip the Vs: rgb.height is zero at this point(!), so use the constant
      mesh.addTexCoord(ofVec2f(tex[0], TEXTURE_HEIGHT - tex[1]));
      mesh.addVertex(ofVec3f(point[0] * scale, point[1] * scale, point[2]));
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

void Frame::render(float rotation[3]){
  ofRectangle fb = face_bounds();
  glPointSize(voxel_size);
  ofPushMatrix();
  // Clip to the face drawing area
  glEnable(GL_SCISSOR_TEST);
  glScissor(fb.x, fb.y, fb.width, fb.height);
  // Centre drawing on the face area
  ofTranslate((fb.x + (fb.width / 2.0)), (fb.y + (fb.height / 2.0)), 0.0);
  ofRotateX(rotation[0]);
  ofRotateY(rotation[1]);
  ofRotateZ(rotation[2]);
  glEnable(GL_DEPTH_TEST);
  rgb.bind();
  mesh.drawVertices();
  rgb.unbind();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  ofPopMatrix();
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
    }
  }
}

// Load emotions from disk into a map of {emotion names : frame vectors}
// Each frame on disk has the name format "seconds.fraction.png",
// with.xyz and .uv files accompanying it

void load_expressions(const std::string & person_dir){
  for(std::vector<std::string>::const_iterator i = emotions.begin();
      i != emotions.end(); ++i){
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

// The rotation to draw the frame at
float rotation[] = {0.0, 0.0, 0.0};

// C++ really doesn't have a float pseudo-random number generator

float frand(){
  return ((float)random()) / (float)RAND_MAX;
}

// Drift one rotation by up to the drifr amount, clamping it to the max rotation

void update_one_rotation(float &rotation){
  float delta = frand() * rotation_drift;
  // Half the time the amount is positive, half the time it's negative
  if(frand() > 0.5){
    delta = - delta;
  }
  float new_rotation = rotation + delta;
  if(std::fabs(new_rotation) <= max_rotation_drift){
    rotation = new_rotation;
  }
}

// Drift the rotation of the frame around the origin

void update_rotation(){
  update_one_rotation(rotation[0]);
  update_one_rotation(rotation[1]);
  // Don't roll around Z, it looks strange
  //update_one_rotation(rotation[2]);
}

// Check whether it's time to update the rotation

bool should_update_rotation(double now){
  static double previous_now = 0.0;
  bool should = false;
  double delta = now - previous_now;
  if(delta > rotation_update_frequency){
    previous_now = now;
    should = true;
  }
  return should;
}

// Set the frame to be rendered from the current emotion at the current time

void update_expression(const std::string & emotion, double now){
  if(should_update_rotation(now)){
    update_rotation();
  }
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
  current_frame.render(rotation);
  // Frame the render (overpainting any stray voxel edges)
  ofRectangle bounds = face_bounds();
  ofNoFill();
  ofRect(bounds);
}
