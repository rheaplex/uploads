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


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

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

#include "ofGraphics.h"
#include <ofImage.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "eeg.h"
#include "emotion.h"
#include "expression.h"
#include "layout.h"


////////////////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////////////////

static const int TEXTURE_WIDTH = 640;
static const int TEXTURE_HEIGHT = 480;

static const float MAX_ROTATION_DRIFT = 0.8f;

static const GLclampf CLEAR_COLOUR[4] = {0, 0, 0, 0};


////////////////////////////////////////////////////////////////////////////////
// File loading
////////////////////////////////////////////////////////////////////////////////

// Load the lines of a gzipped text file into a vector of strings

void slurp_gzipped_lines(const std::string & path,
			 std::vector<std::string> & lines)
{
  std::ifstream file(path);//, ios_base::in | ios_base::binary);
  boost::iostreams::filtering_stream<boost::iostreams::input> in;
  in.push(boost::iostreams::gzip_decompressor());
  in.push(file);
  std::string line;
  while(std::getline(in, line))
  {
    lines.push_back(line);
  }
}

// Load the values of a gzipped tsv file into an array of floats
// Caller owns the new[] float array

size_t slurp_gzipped_csv_floats(boost::shared_array<float> & values,
				const std::string & path,
				int stride)
{
  std::vector<std::string> lines;
  slurp_gzipped_lines(path, lines);
  values = boost::shared_array<float>(new float[lines.size() * stride]);
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("\t");
  for(size_t i = 0; i < lines.size(); i++)
  {
    tokenizer::iterator tokens = tokenizer(lines[i], sep).begin();
    int index = i * stride;
    for(int j = 0; j < stride; j++)
    {
      values[index + j] = std::atof((*tokens).c_str());
      tokens++;
    }
  }
  return lines.size();
}


////////////////////////////////////////////////////////////////////////////////
// Frames
////////////////////////////////////////////////////////////////////////////////

class Frame
{
public:
  Frame();
  Frame(const boost::filesystem::path & path_root, double when_base);
  ~Frame();
  void render(float rotation[3]);

public:
  double when;

private:
  // So we can copy Frames into STL containers
  boost::shared_array<float> xyz;
  boost::shared_array<float> uv;
  size_t num_coords;
  ofTexture rgb;
};

std::map<std::string, std::vector<Frame> > expression_frames;
std::map<std::string, float> expression_ranges;

Frame::Frame() :
  num_coords(0)
{
}

// Load the frame from the path

Frame::Frame(const boost::filesystem::path & path_root, double when_base)
{
  // The path is of the format /a/b/c/2346.12
  when = std::atof(path_root.filename().c_str()) - when_base;
  num_coords = slurp_gzipped_csv_floats(xyz, path_root.string() + ".xyz", 3);
  size_t other_num_coords = slurp_gzipped_csv_floats(uv,
						     path_root.string() + ".uv",
						     2);
  // Move this to a non-debug check
  assert(num_coords == other_num_coords);
  ofLoadImage(rgb, path_root.string() + ".png");
}

Frame::~Frame()
{
}

// Render the frame in the OpenGL context

void Frame::render(float rotation[3])
{
  //rgb.draw(0, 0, 200, 200);
 /*::glClearColor(CLEAR_COLOUR[0], CLEAR_COLOUR[1], CLEAR_COLOUR[2],
    CLEAR_COLOUR[3]);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
  ::glEnable(GL_DEPTH_TEST);

  ::glPushMatrix();

  ::glMatrixMode(GL_PROJECTION);
  ::glLoadIdentity();
  ::gluPerspective(60, 4.0 / 3.0, 0.3, 200);
  ::glMatrixMode(GL_MODELVIEW);
  ::glLoadIdentity();

  ::glTranslatef(0, 0, -2.0);

  ::glMatrixMode(GL_TEXTURE);
  ::glLoadIdentity();
  ::glMatrixMode(GL_MODELVIEW);
  ::glPushMatrix();

  ::glVertexPointer(4, GL_FLOAT, 0, xyz.get());
  ::glTexCoordPointer(4, GL_FLOAT, 0, uv.get());

  ::glPointSize(2);
  ::glEnableClientState(GL_VERTEX_ARRAY);
  ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  ::glColor3f(1, 1, 1);
  ::glDrawArrays(GL_POINTS, 0, num_coords);
  ::glDisableClientState(GL_VERTEX_ARRAY);
  ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  ::glPopMatrix();
  ::glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////
// Emotion loading
////////////////////////////////////////////////////////////////////////////////

// Predicate to check whether a file path is a png file (or at least ends .png)

static const boost::regex png_filter( ".*\\.png" );

bool is_png_file(const boost::filesystem::directory_iterator & i)
{
  bool is = true;
  if(! boost::filesystem::is_regular_file(i->status()))
  {
    is = false;
  }
  boost::smatch what;
  if(! boost::regex_match(i->leaf(), what, png_filter))
  {
      is = false;
  }
  return is;
}

// Load all the frames for an emotion directory into a vector

void load_expression(const std::string & emotion_dir,
		     std::vector<Frame> & frames)
{
  std::cerr << emotion_dir << std::endl;
  boost::filesystem::directory_iterator end;
  boost::filesystem::directory_iterator i(emotion_dir);
  double when_base = -1.0;
  // Load each frame
  for(; i != end; ++i )
  {
    // use .pngs as the indicators of times. alphabetically they're first
    if (is_png_file(i))
    {
      // Strip the .png extention to get a base /path/time.stamp
      boost::filesystem::path path(i->path());
      path.replace_extension("");
      std::cerr << path.string() << std::endl;
      // We need to set the base timestamp from the first png filename
      if(when_base == -1.0)
      {
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

void load_expressions(const std::string & person_dir)
{
  for(std::vector<std::string>::const_iterator i = emotions.begin();
      i != emotions.end(); ++i)
  {
    std::vector<Frame> frames;
    load_expression(person_dir + "/" + *i, frames);
    expression_frames[*i] = frames;
    expression_ranges[*i] = frames.back().when;
  }
}

static Frame current_frame;

void update_expression(const std::string & emotion, double now)
{
  // Make sure the value is in range
  double now_mod = std::fmod(now, expression_frames[emotion].rbegin()->when);
  std::vector<Frame>::iterator i = expression_frames[emotion].begin();
  while(i->when < now_mod)
    {
      ++i;
      //REMOVE: sloooooooow. Remove as soon as this works
      assert(i != expression_frames[emotion].end());
    }
  current_frame = *i;
}

void draw_expression()
{
  ofRectangle bounds = face_bounds();
  ofRect(bounds);
  float rotation[] = {0.0, 0.0, 0.0};
  current_frame.render(rotation);
}
