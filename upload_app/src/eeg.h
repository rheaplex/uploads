////////////////////////////////////////////////////////////////////////////////
//    eeg.h - load serialized eeg data
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

#ifndef __EEG_H__
#define __EEG_H__

#include <deque>
#include <map>
#include <string>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// EEG Power Levels
////////////////////////////////////////////////////////////////////////////////

struct power_levels
{
  // The indexes in the values
  enum
    {
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

typedef std::vector<power_levels> power_levels_vector;
typedef power_levels_vector::iterator power_levels_iterator;

extern std::vector<std::string> values_names;

////////////////////////////////////////////////////////////////////////////////
// Raw EEG data
////////////////////////////////////////////////////////////////////////////////

struct raw_eeg
{
  double timestamp;
  int value;
};

typedef std::vector<raw_eeg> raw_eeg_vector;
typedef raw_eeg_vector::iterator raw_eeg_iterator;


////////////////////////////////////////////////////////////////////////////////
// Eeg data for emotions
////////////////////////////////////////////////////////////////////////////////

// The data for an emotion

struct emotion_data
{
  std::string name;
  raw_eeg_vector raw_eeg;
  power_levels_vector power_levels;
  double max_levels_time;
};

// A map of emotion names to emotion data objects

typedef std::map<std::string, emotion_data> emotion_data_map;

// Load EEG datas from file

void  load_emotions(const std::string & name);


////////////////////////////////////////////////////////////////////////////////
// Current data state for plotting
////////////////////////////////////////////////////////////////////////////////

void update_state();
double update_display_data();

extern std::deque<power_levels> levels_display_data;
extern std::deque<raw_eeg> eeg_display_data;


#endif
