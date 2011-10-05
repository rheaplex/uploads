#ifndef __EEG_H__
#define __EEG_H_

#include 

////////////////////////////////////////////////////////////////////////////////
// EEG Power Levels
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
typedef power_levels_vector::iterator power_levels_iterator;


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

#endif
