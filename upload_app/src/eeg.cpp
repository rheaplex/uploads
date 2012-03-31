////////////////////////////////////////////////////////////////////////////////
//    eeg.cpp - load serialized eeg data
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "ofGraphics.h"
#include "ofRectangle.h"

#include "emotion.h"
#include "layout.h"

#include "eeg.h"


////////////////////////////////////////////////////////////////////////////////
// Timestamps
////////////////////////////////////////////////////////////////////////////////

// Normalize timestamps in the sample objects in a vector to be relative to
// the first as time 0.0

template<class T>
void normalize_timestamps(T & values, double base){
  for(typename T::iterator i = values.begin(); i < values.end(); ++i){
    (*i).timestamp = (*i).timestamp - base;
  }
}

template<class T>
void sort_by_timestamps(std::vector<T> & values){
  std::sort(values.begin(), values.end(),
	    [](const T & a, const T & b){return a.timestamp < b.timestamp;});
}


////////////////////////////////////////////////////////////////////////////////
// EEG Power Levels
// These are the processed EEG readings, one set per second
////////////////////////////////////////////////////////////////////////////////

struct power_levels {
  // The indexes in the values
  enum {
      poor_signal_level=0,
      low_alpha,
      high_alpha,
      low_beta,
      high_beta,
      low_gamma,
      high_gamma,
      attention,
      meditation
  };

  double timestamp;

  // Because indexing these takes 1/10th the code of accessing named fields
  int values[9];
};

// A vector of power level samples for an emotion

typedef std::vector<power_levels> power_levels_vector;
typedef power_levels_vector::iterator power_levels_iterator;

// The names for the power level samples

std::vector<std::string> values_names = 
  {"poor signal level", "low alpha", "high alpha", "low beta", "high beta",
   "low gamma", "high gamma", "attention", "meditation"};

// An empty power levels object to pre-populate a vector with

const power_levels dummy_power_levels = {0.0, {0,0,0,0,0,0,0,0,0}};

// Create a power_levels object from the values in a line of data

void line_to_levels(const std::string & line, power_levels & levels){
  std::vector<std::string> fields;
  boost::split(fields, line, boost::is_any_of("\t"));
  levels.timestamp = ::atof(fields[0].c_str());
  levels.values[0] = ::atoi(fields[1].c_str());
  levels.values[1] = ::atoi(fields[2].c_str());
  levels.values[2] = ::atoi(fields[3].c_str());
  levels.values[3] = ::atoi(fields[4].c_str());
  levels.values[4] = ::atoi(fields[5].c_str());
  levels.values[5] = ::atoi(fields[6].c_str());
  levels.values[6] = ::atoi(fields[7].c_str());
  levels.values[7] = ::atoi(fields[8].c_str());
  levels.values[8] = ::atoi(fields[9].c_str());
  assert(levels.timestamp != 0.0);
}

// Slurp power_levels from a tsv file into a vector

void slurp_power_levels(const std::string & file,
			power_levels_vector & levels_vector){
  std::ifstream source(file);
  if(!source.is_open()){
    throw std::runtime_error("Couldn't open file " + file);
  }
  while(source){
    std::string line;
    std::getline(source, line);
    // Skip comments
    if(line.length() > 0 && line[0] == '#'){
      continue;
    }
    // We get an empty line at the end???
    if(line.length() == 0){
      break;
    }
    power_levels levels;
    line_to_levels(line, levels);
    levels_vector.push_back(levels);	      
  }
}


////////////////////////////////////////////////////////////////////////////////
// Raw EEG data
// These are the raw EEG readings, many per second
////////////////////////////////////////////////////////////////////////////////

struct raw_eeg {
  double timestamp;
  int value;
};

// A vector of raw eeg values for an emotion

typedef std::vector<raw_eeg> raw_eeg_vector;
typedef raw_eeg_vector::iterator raw_eeg_iterator;

const raw_eeg dummy_raw_eeg = {0.0, 0};

// Slurp raw eeg data from a tsv file into a vector

void slurp_raw_eeg(const std::string & file, raw_eeg_vector & eeg_vector){
  std::ifstream source(file);
  if(!source.is_open()){
    throw std::runtime_error("Couldn't open file " + file);
  }
  while(source){
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
    raw_eeg eeg = {atof(fields[0].c_str()), atoi(fields[1].c_str())};
    assert(eeg.timestamp != 0.0);
    eeg_vector.push_back(eeg);				      
  }
}

// Truncate raw eeg samples (e) to same timestamp range as power levels (L)
// so if we start with LeeeeeeeLeeeeeLeeeeeLee
//  we should get: LeeeeeeeLeeeeeLeeeeeL

void truncate_eeg_to_levels_timestamps(raw_eeg_vector & eeg, 
				       power_levels_vector & levels){
  double levels_begin = levels.front().timestamp;
  double levels_end = levels.back().timestamp;
  // incredibly inefficient. find_if & rbegin then truncate isn't happy on gcc4
  std::remove_if(eeg.begin(), eeg.end(), 
		 [&levels_begin](raw_eeg & e)
		 {return e.timestamp < levels_begin;});
  std::find_if(eeg.begin(), eeg.end(), 
	       [&levels_end](raw_eeg & e)
	       {return e.timestamp > levels_end;});
}


////////////////////////////////////////////////////////////////////////////////
// The map of emotions to eeg data
////////////////////////////////////////////////////////////////////////////////

// The data for an emotion

struct emotion_data {
  std::string name;
  raw_eeg_vector raw_eeg;
  power_levels_vector power_levels;
  double max_levels_time;
};

// A map of emotion names to emotion data objects

typedef std::map<std::string, emotion_data> emotion_data_map;

// The map of emotion names to eeg data

emotion_data_map emotion_datas;

// The names of the files in each emotion directory

const std::string RAW_EEG_FILENAME("raw_eeg.txt");
const std::string POWER_LEVELS_FILENAME("power_levels.txt");

// Make the path to the relevant file for the relevant emotion for the user

std::string emotion_file(const std::string & user_name,
			 const std::string & emotion,
			 const std::string & data_file){
  return user_name + "/" + emotion + "/" + data_file;
}

// Load the raw eeg data file for the emotion for the user

void load_emotion_raw_eeg_data(const std::string & user_name,
			       const std::string & emotion,
			       raw_eeg_vector & eeg_vector){
  slurp_raw_eeg(emotion_file(user_name, emotion, RAW_EEG_FILENAME),
		eeg_vector);
}

// Load the power levels data file for the emotion for the user

void load_emotion_power_levels_data(const std::string & user_name,
				    const std::string & emotion,
				    power_levels_vector & levels_vector){
  slurp_power_levels(emotion_file(user_name, emotion, POWER_LEVELS_FILENAME),
		     levels_vector);
}

// Load all the user's emotion data files from each emotion folder

void load_emotions(const std::string & username){
  BOOST_FOREACH(const std::string & emotion, emotions){
    emotion_data data;
    data.name = emotion;
    load_emotion_power_levels_data(username, emotion, data.power_levels);
    load_emotion_raw_eeg_data(username, emotion, data.raw_eeg);
    // Ensure power levels and raw eeg data have the same range
    truncate_eeg_to_levels_timestamps(data.raw_eeg, data.power_levels);
    // Ensure they are in order
    sort_by_timestamps(data.power_levels);
    sort_by_timestamps(data.raw_eeg);
    // Then make the timestamps on each relative to the first timestamp
    double relative_to = data.power_levels[0].timestamp;
    normalize_timestamps(data.power_levels, relative_to);
    normalize_timestamps(data.raw_eeg, relative_to);
    data.max_levels_time = data.power_levels.back().timestamp;
    emotion_datas[emotion] = data;
  }
}


////////////////////////////////////////////////////////////////////////////////
// The current EEG data for drawing
////////////////////////////////////////////////////////////////////////////////

// The number of items of data to plot

static const unsigned int levels_to_display = 30;
static const unsigned int eeg_to_display = 200;

// The current time

double last_update_at = 0.0;

// The current emotion, from Twitter

emotion_data current_emotion_data;

// The current positions in the current emotion's data

raw_eeg_iterator current_eeg_iterator;
power_levels_iterator current_levels_iterator;

// The data to plot visually

std::deque<power_levels> levels_display_data(levels_to_display,
					     dummy_power_levels);
std::deque<raw_eeg> eeg_display_data(eeg_to_display, dummy_raw_eeg);

// Set a data item iterator to the current time
// Used in initialization and when the current emotion is changed

template<class T>
typename T::iterator set_current_iterator(float current_time_mod, T & source){
  typename T::iterator iter = source.begin();
  while((*iter).timestamp < current_time_mod){
    ++iter;
  }
  return iter;
}

// Take data from the source vector and push into the destination vector
// This wraps round carefully
// But it won't handle multiple loops through the data
// so make sure update times fit into sample times

template<class T, class U>
void update_data_vector(double now_mod,
			double previous_mod,
			T & source,
			U & destination,
			typename T::iterator & iter){
  // If we've wrapped round
  // In pathological cases of system delay we will miss multiple passes
  //  if that happens there are probably worse things to worry about
  //  and the code won't fail, it
  if(now_mod < previous_mod){ 
    // Push the remaining values
    for(; iter != source.end(); ++iter){
      destination.push_front(*iter);
    }
    // and wrap round
    iter = source.begin();	
  }
  // Whether we looped or not, keep pushing values until we hit the current time
  while(iter->timestamp <= now_mod){
    // Push the new value
    destination.push_front(*iter);
    // Move on to the next data item
    ++iter;	
  }
  // Note that the iterator is now pointing to the next item,
  //  which means it will be at a time *after* the current time
}


////////////////////////////////////////////////////////////////////////////////
// Update the drawing data as time elapses
////////////////////////////////////////////////////////////////////////////////

std::string previous_emotion = "";

// Get the current time in the range 0..max_timestamp

static double mod_time(emotion_data & data, double now){
  return std::fmod(now, data.max_levels_time);
}

// update the display data
// the strategy is to push data until the iterator catches up to now,
//  then truncate the display data if too long
// note that our use of globals makes this non-threadsafe

void update_eegs(const std::string & emotion, double raw_time){
  // Handle being called before Twitter streaming has started
  if(emotion == ""){
    return;
  }
  if(emotion != previous_emotion){
    current_emotion_data = emotion_datas[emotion];
    current_eeg_iterator =
      set_current_iterator(last_update_at, current_emotion_data.raw_eeg);
    current_levels_iterator =
      set_current_iterator(last_update_at,current_emotion_data.power_levels);
    previous_emotion = emotion;
  }
  double current_update_at = mod_time(current_emotion_data, raw_time);
  // Push new data
  update_data_vector(current_update_at, last_update_at,
  		     current_emotion_data.raw_eeg, eeg_display_data,
  		     current_eeg_iterator);
  update_data_vector(current_update_at, last_update_at,
		     current_emotion_data.power_levels,
		     levels_display_data, current_levels_iterator);
  // Truncate old data
  eeg_display_data.resize(std::min(static_cast<unsigned int>
				   (eeg_display_data.size()), eeg_to_display));
  levels_display_data.resize(std::min(static_cast<unsigned int>
				      (levels_display_data.size()),
				      levels_to_display));
  last_update_at = current_update_at;
}


////////////////////////////////////////////////////////////////////////////////
// Draw the current eeg data
////////////////////////////////////////////////////////////////////////////////

// Min/max values for various items

static const int eeg_min = -2048;
static const int eeg_max = 2048;
static const double eeg_scale = 1.0 / (eeg_max - eeg_min);
static const int attention_meditation_min = 0;
static const int attention_meditation_max = 100;
static const int levels_min = 0;
// 24-bit MAX_INT
static const int levels_max = 16777215;
static const double levels_scale = 1.0 / (levels_max - levels_min);

// The label for the Raw EEG plot

std::string raw_label("raw eeg");

// Draw the frame for a wave plot

void frame_wave(int index){
  ofRectangle & frame = eeg_bounds(index);
  ofRect(frame.x, frame.y, frame.width, frame.height);
}

// Plot a waveform

void draw_wave(int index, size_t count,
	      double (*accessor)(int index, size_t i)){
  ofRectangle & frame = eeg_bounds(index);
  float step = frame.width / (count - 1);
  float x = frame.x;
  ofBeginShape();
  for(size_t i = 0; i < count; i++){
    // Subtract value from 1.0 so 0 is at the bottom of the drawing area
    float y = frame.y + (frame.height * (1.0 - accessor(index, i)));
    ofVertex(x, y);
    x += step;
  }
  ofEndShape();
}

void draw_label(int index, std::string & label){
  ofRectangle & frame = eeg_bounds(index);
  ofDrawBitmapString(label, frame.x,
		     frame.y + frame.height + label_size_small());
}

// Draw the raw eeg and the primary processed values

void draw_eegs(){
  // Common graphics state setup
  ofEnableSmoothing();
  ofNoFill();
  ofColor fore;
  foreground_colour(fore);
  ofSetColor(fore);
  // Graphics state for frames
  ofSetLineWidth(frame_line_width());
  for(int i = 0; i < EEG_COUNT; i++){
      frame_wave(i);
  }
  // Graphics state for eeg traces
  ofSetLineWidth(eeg_line_width());
  // Draw the raw values separately
  draw_label(0, raw_label);
  draw_wave(0, eeg_display_data.size(),
	    [](int index, size_t i){
	      return eeg_scale * (eeg_display_data[i].value - eeg_min);});
  // Draw the calculated values
  for(int i = 1; i < EEG_COUNT; i++){
    draw_label(i, values_names[i + EEG_LOW - 1]);
  }
  for(int i = EEG_LOW; i < EEG_ABOVE; i++){
    draw_wave(i - EEG_LOW, levels_display_data.size(),
	      [](int index, size_t i){
		return levels_scale * levels_display_data[i].values[index];});
  }
}
