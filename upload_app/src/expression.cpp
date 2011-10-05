
////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/tokenizer.hpp>

#include <GL/GL.h>
#include <GLU/GLU.h>

#include <ofImage.h>


////////////////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////////////////

static const int TEXTURE_WIDTH = 640;
static const int TEXTURE_HEIGHT = 480;

static const float MAX_ROTATION_DRIFT = 0.8f;

static const GLclampf CLEAR_COLOUR[3] = {0, 0, 0};


////////////////////////////////////////////////////////////////////////////////
// File loading
////////////////////////////////////////////////////////////////////////////////

// Load the lines of a gzipped text file into a vector of strings

void slurp_gzipped_lines(const std::string & path,
			 std::vector<std::string> & lines)
{
  std::ifstream file(path, ios_base::in | ios_base::binary);
  filtering_streambuf<input> in;
  in.push(gzip_decompressor());
  in.push(file);
  std::string line;
  while(getline(in, line))
  {
    lines.push_back(line);
  }
}

// Load the values of a gzipped tsv file into an array of floats
// Caller owns the new[] array

float * slurp_gzipped_csv_floats(const std::string & path, int stride)
{
  std::vector<std::string> lines;
  slurp_gzipped_lines(path, lines);
  float * values = new float[lines.size() * stride];
  for(int i = 0; i < lines.size(); i++)
  {
    tokenizer<>::iterator tokens = tokenizer<>(lines[i])::begin();
    int index = i * stride;
    for(int j = 0; j < stride; j++)
    {
      values[index + j] = *tokens;
      tokens++;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Frames
////////////////////////////////////////////////////////////////////////////////

struct Frame
{
  Frame();
  ~Frame();
  void render(float rotation[3]);

  ofImage rgb;
  float * xyz;
  float * uv
  float when;
};

Frame::Frame(const std::string & path_root, float when) :
  when(when),
  xyz(slurp_gzipped_csv_floats(path_root + ".xyz", 3)),
  uv(slurp_gzipped_csv_floats(path_root + ".uv", 2))
{
  rgb.load(path_root + ".png");
}

Frame::~Frame()
{
  if(rgb) delete[] rgb;
  if(xyz) delete[] xyz;
  if(uv) delete[] uv;
}

void Frame::render(float rotation[3])
{
  ::glClearColor(CLEAR_COLOUR[0], CLEAR_COLOUR[1], CLEAR_COLOUR[2]);
  ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ::glEnable(GL_DEPTH_TEST);

  ::glPushMatrix();

  ::glMatrixMode(GL_PROJECTION);
  ::glLoadIdentity();
  ::gluPerspective(60, 4.0 / 3.0, 0.3, 200);
  ::glMatrixMode(GL_MODELVIEW);
  ::glLoadIdentity();

  //::glScale(zoomdist,zoomdist,1)
  ::glTranslatef(0, 0, -3.5);
  ::glRotatef(rotation[0], 1.0, 0.0, 0.0);
  ::glRotatef(rotation[1], 0.0, 1.0, 0.0);
  ::glRotatef(rotation[2], 0.0, 0.0, 1.0);
  ::glTranslatef(0, 0, 1.5);

  ::glMatrixMode(GL_TEXTURE);
  ::glLoadIdentity();
  ::glMatrixMode(GL_MODELVIEW);
  ::glPushMatrix();
  ::glVertexPointerf(xyz);
  ::glTexCoordPointerf(uv);

  ::glPointSize(2);
  ::glEnableClientState(GL_VERTEX_ARRAY);
  ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  ::glEnable(TEXTURE_TARGET);
  ::glColor3f(1, 1, 1);
  ::glDrawElements(GL_POINTS, xyz.size());
  ::glDisableClientState(GL_VERTEX_ARRAY);
  ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  ::glDisable(TEXTURE_TARGET);
  ::glPopMatrix();
  ::glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////
// Emotion loading
////////////////////////////////////////////////////////////////////////////////

// Predicate to check whether a file path is a png file (or at least ends .png)

static const boost::regex png_filter( ".*\.png" );

void is_png_file(const boost::filesystem::directory_iterator & i)
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

// Load the expression frames

void load_expression(const std::string & emotion_dir,
		     std::vector<Frame> & frames)
{
  boost::filesystem::directory_iterator end; 
  for(boost::filesystem::directory_iterator i(emotion_dir); i != end; ++i )
  {
    if (is_png_file(i))
    {
      std::string::size_type dot = i->leaf().find_last_of('.');
      // if dot == npos we want to crash, really...
      std::string frame_path_root = i->leaf().substr(0, end + 1);
      Frame frame(frame_path_root);
      frames.push_back(frame);
    }
  }
}

// Load all the expression frames into a map of emotion names : frame vectors

void load_expressions(const std::string & person_dir,
		      std::map<std::string, std::vector<Frame> > & expressions)
{
  for(std::vector<std::string> i = emotions.begin; i != emotions.end(); ++i)
  {
    std::vector<Frame> frames;
    load_expression(person_dir + "/" + *i, frames);
    expressions[*i] = frames;
  }
}
