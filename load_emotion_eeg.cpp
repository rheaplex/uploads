////////////////////////////////////////////////////////////////////////////////
//    twitter_streaming_search - Scan Twitter for mentions of emotions
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
// Notes
////////////////////////////////////////////////////////////////////////////////

//  Data loaded from file largely using C functions and represented as 1d arrays
// This is more efficient for both cpu and developer cycles.

// We use c++0x, which needs specifying:
// g++ -std=c++0x -lcurl -o emoload load_emotion_eeg.cpp

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

////////////////////////////////////////////////////////////////////////////////
// File slurping
////////////////////////////////////////////////////////////////////////////////

// Slurp a file into a byte buffer in memory

template<class T>
void slurp(const std::string & filename, T *& buffer,
	   unsigned int & count)
{
  int filenum = ::open(filename.c_str(), O_RDONLY);
  if(filenum == -1)
    throw std::runtime_error("No such file");
  struct stat stat_details;
  int status = ::fstat(filenum, &stat_details);
  if(status == -1)
    throw std::runtime_error("Couldn't stat file");
  size_t buffer_size = stat_details.st_size;
  count = buffer_size / sizeof(T);
  buffer = new T[buffer_size];
  int result = ::read(filenum, buffer, buffer_size);
  if(result != buffer_size)
    throw std::runtime_error("Didn't read entire file");
}


////////////////////////////////////////////////////////////////////////////////
// Raw EEG data
////////////////////////////////////////////////////////////////////////////////

enum raw_eeg_fields{RAW_EEG_TIMESTAMP_FIELD = 0,
		    RAW_EEG_VALUE_FIELD,
		    RAW_EEG_FIELD_COUNT};

// slurp raw eeg data into a 1d array of floats, calculating the number of rows

void slurp_raw_eeg(const std::string & file, float *& eeg,
		   unsigned int & row_count)
{
  unsigned int floats_count;
  slurp(file, eeg, floats_count);
  row_count = floats_count / RAW_EEG_FIELD_COUNT;
}

// Get the start of a row of eeg data in a 1d array of floats

static float * raw_eeg_row(float * buffer, unsigned int index)
{
  return buffer + (index * RAW_EEG_FIELD_COUNT);
}


////////////////////////////////////////////////////////////////////////////////
// Power Levels
////////////////////////////////////////////////////////////////////////////////

enum power_level_fields{POWER_LEVELS_TIMESTAMP_FIELD = 0,
			POWER_LEVELS_POOR_SIGNAL_LEVEL,
			POWER_LEVELS_LOW_ALPHA,
			POWER_LEVELS_HIGH_ALPHA,
			POWER_LEVELS_LOW_BETA,
			POWER_LEVELS_HIGH_BETA,
			POWER_LEVELS_LOW_GAMMA,
			POWER_LEVELS_HIGH_GAMMA,
			POWER_LEVELS_ATTENTION,
			POWER_LEVELS_MEDITATION,
			POWER_LEVELS_FIELD_COUNT};

// slurp raw power_levels into 1d array of floats, calculating number of rows

void slurp_power_levels(const std::string & file, float *& eeg,
			unsigned int & row_count)
{
  unsigned int floats_count;
  slurp(file, eeg, floats_count);
  row_count = floats_count / POWER_LEVELS_FIELD_COUNT;
}

// Get the start of a row of power levels in a 1d array of floats

static float * power_levels_row(float * buffer, unsigned int index)
{
  return buffer + (index * POWER_LEVELS_FIELD_COUNT);
}


////////////////////////////////////////////////////////////////////////////////
// Emotion files
////////////////////////////////////////////////////////////////////////////////

// Ekman's basic emotions

//const std::vector<std::string> emotions = {"anger", "disgust", "fear", "happiness", "sadness", "surprise"};

// Descartes' basic emotions

const std::vector<std::string> emotions = {"wonder", "love", "hatred", "desire", "joy", "sadness"};

// The names of the files for each emotion

const std::string RAW_EEG_FILENAME("raw_eeg.bin");
const std::string POWER_LEVELS_FILENAME("power_levels.bin");

// Make the path to the relevant file for the relevant emotion for the user

std::string emotion_file(const std::string & user_name,
			 const std::string & emotion,
			 const std::string & data_file)
{
  return user_name + "/" + emotion + "/" + data_file;
}

// Load the raw eeg data file for the emotion for the user

void load_emotion_raw_eeg_data(const std::string & user_name,
			       const std::string & emotion,
			       float *& raw_eeg,
			       unsigned int & raw_eeg_rows)
{
  slurp_raw_eeg(emotion_file(user_name, emotion, RAW_EEG_FILENAME),
		raw_eeg, raw_eeg_rows);
}

// Load the power levels data file for the emotion for the user

void load_emotion_power_levels_data(const std::string & user_name,
				    const std::string & emotion,
				    float *& power_levels,
				    unsigned int & power_levels_rows)
{
  slurp_power_levels(emotion_file(user_name, emotion, POWER_LEVELS_FILENAME),
		     power_levels, power_levels_rows);
}

// The data for an emotion

struct emotion_data
{
  float * raw_eeg_data;
  unsigned int raw_eeg_data_rows;
  float * power_levels;
  unsigned int power_levels_rows;
};

// A map of emotion names to emotion data objects

typedef std::map<std::string, emotion_data> emotion_data_map;

// Load all the user's emotion data files from each emotion folder

void load_emotions(emotion_data_map & emotion_map, const std::string & username)
{
  BOOST_FOREACH(const std::string & emotion, emotions)
  {
    emotion_data data;
    load_emotion_power_levels_data(username, emotion, data.power_levels,
				   data.power_levels_rows);
    load_emotion_raw_eeg_data(username, emotion, data.raw_eeg_data,
			      data.raw_eeg_data_rows);
    emotion_map[emotion] = data;
  }
}


////////////////////////////////////////////////////////////////////////////////
// Main flow of control
////////////////////////////////////////////////////////////////////////////////

// Until we get a command line option processor done, just check simply

void check_args(const int argc, const char * argv[])
{
  if(argc != 2)
  {
    std::cerr << "USAGE: pass user emotions folder path as first argument"
	      << std::endl;
    std::exit(1);
  }
}

// Do everything

int main(int argc, const char * argv[])
{
  check_args(argc, argv);
  const char * name = argv[1];
  emotion_data_map emotion_datas;
  load_emotions(emotion_datas, name);
  emotion_data & emo = emotion_datas[emotions[0]];
  std::cout << emotions[0] << std::endl;
  for(int i = 0; i < 2; i++)
  {
    float * row = power_levels_row(emo.power_levels, i);
    for(int j = 0; j < POWER_LEVELS_FIELD_COUNT; j++)
    {
      std::cout << std::fixed << row[j];
      std::cout << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}
