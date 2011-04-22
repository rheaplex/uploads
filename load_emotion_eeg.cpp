/*
  NOTES:
  
  Data loaded from file is represented as 1d arrays of values.
  This is more efficient for both cpu and developer cycles.

*/


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <string>

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

static const unsigned int RAW_EEG_TIMESTAMP_FIELD = 0;
static const unsigned int RAW_EEG_VALUE_FIELD = 1;
static const unsigned int RAW_EEG_FIELD_COUNT = 2;

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
// Main flow of control
////////////////////////////////////////////////////////////////////////////////

// test raw eeg slurping

int main(int argc, char * argv[])
{
  float * floats;
  unsigned int floats_rows;
  slurp_raw_eeg(argv[1], floats, floats_rows);
  for(int i = 0; i < floats_rows; i++)
    {
      float * row = raw_eeg_row(floats, i);
      std::cerr << i << " "<< row[RAW_EEG_TIMESTAMP_FIELD] << " " 
		<< row[RAW_EEG_VALUE_FIELD] << std::endl;
    }
  return 0;
}
