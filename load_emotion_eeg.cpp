////////////////////////////////////////////////////////////////////////////////
//    load_emotion_eeg - load serialized eeg data
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

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>


////////////////////////////////////////////////////////////////////////////////
// Raw EEG data
////////////////////////////////////////////////////////////////////////////////

struct raw_eeg
{
  double timestamp;
  int value;
};

typedef std::vector<raw_eeg> raw_eeg_vector;

// slurp raw eeg data into a 1d array of floats, calculating the number of rows

void slurp_raw_eeg(const std::string & file, raw_eeg_vector & eeg_vector)
{
  std::ifstream source(file);
  while(source)
  {
    std::string line;
    std::getline(source, line);
    // Skip comments
    if(line.length() > 0 && line[0] == '#')
      continue;
    // We get an empty line at the end???
    if(line.length() == 0)
      break;
    std::vector<std::string> fields;
    boost::split(fields, line, boost::is_any_of("\t"));
    raw_eeg eeg = {
      ::atof(fields[0].c_str()), ::atoi(fields[1].c_str())
    };
    assert(eeg.timestamp != 0.0);
    eeg_vector.push_back(eeg);				      
  }
}


////////////////////////////////////////////////////////////////////////////////
// Power Levels
////////////////////////////////////////////////////////////////////////////////

struct power_levels
{
  double timestamp;
  int poor_signal_level;
  int low_alpha;
  int high_alpha;
  int low_beta;
  int high_beta;
  int low_gamma;
  int high_gamma;
  int attention;
  int meditation;
};

typedef std::vector<power_levels> power_levels_vector;

// slurp raw power_levels into 1d array of floats, calculating number of rows

void slurp_power_levels(const std::string & file,
			power_levels_vector & levels_vector)
{
  std::ifstream source(file);
  while(source)
  {
    std::string line;
    std::getline(source, line);
    // Skip comments
    if(line.length() > 0 && line[0] == '#')
      continue;
    // We get an empty line at the end???
    if(line.length() == 0)
      break;
    std::vector<std::string> fields;
    boost::split(fields, line, boost::is_any_of("\t"));
    power_levels levels = {
      ::atof(fields[0].c_str()), ::atoi(fields[1].c_str()), 
      ::atoi(fields[2].c_str()), ::atoi(fields[4].c_str()),
      ::atoi(fields[4].c_str()), ::atoi(fields[5].c_str()),
      ::atoi(fields[6].c_str()), ::atoi(fields[7].c_str()),
      ::atoi(fields[8].c_str()), ::atoi(fields[9].c_str())
    };
    assert(levels.timestamp != 0.0);
    levels_vector.push_back(levels);				      
  }
}



////////////////////////////////////////////////////////////////////////////////
// Emotion files
////////////////////////////////////////////////////////////////////////////////

// Ekman's basic emotions

//const std::vector<std::string> emotions = {"anger", "disgust", "fear", "happiness", "sadness", "surprise"};

// Descartes' basic emotions

const std::vector<std::string> emotions = {"wonder", "love", "hatred", "desire", "joy", "sadness"};

// The names of the files for each emotion

const std::string RAW_EEG_FILENAME("raw_eeg.txt");
const std::string POWER_LEVELS_FILENAME("power_levels.txt");

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
			       raw_eeg_vector & eeg_vector)
{
  slurp_raw_eeg(emotion_file(user_name, emotion, RAW_EEG_FILENAME),
		eeg_vector);
}

// Load the power levels data file for the emotion for the user

void load_emotion_power_levels_data(const std::string & user_name,
				    const std::string & emotion,
				    power_levels_vector & levels_vector)
{
  slurp_power_levels(emotion_file(user_name, emotion, POWER_LEVELS_FILENAME),
		     levels_vector);
}

// The data for an emotion

struct emotion_data
{
  raw_eeg_vector raw_eeg;
  power_levels_vector power_levels;
};

// A map of emotion names to emotion data objects

typedef std::map<std::string, emotion_data> emotion_data_map;

// Load all the user's emotion data files from each emotion folder

void load_emotions(emotion_data_map & emotion_map, const std::string & username)
{
  BOOST_FOREACH(const std::string & emotion, emotions)
  {
    emotion_data data;
    load_emotion_power_levels_data(username, emotion, data.power_levels);
    load_emotion_raw_eeg_data(username, emotion, data.raw_eeg);
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

#include <cstdio>

int main(int argc, const char * argv[])
{
  check_args(argc, argv);
  const char * name = argv[1];
  emotion_data_map emotion_datas;
  load_emotions(emotion_datas, name);
  emotion_data & emo = emotion_datas[emotions[0]];
  std::cout << emotions[0] << std::endl;
  power_levels_vector & levels = emo.power_levels;
  for(int i = 0; i < 2; i++)
  {
    std::cout << std::fixed << levels[i].timestamp << " " 
	      << levels[i].low_gamma << std::endl;
  }
  return 0;
}
