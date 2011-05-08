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
// includes
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <stdlib.h>

#include <curl/curl.h>
#include <pthread.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

#include "twitterStreaming.h"


////////////////////////////////////////////////////////////////////////////////
// Emotions
////////////////////////////////////////////////////////////////////////////////

// Ekman's basic emotions

//const std::vector<std::string> emotions = {"anger", "disgust", "fear", "happiness", "sadness", "surprise"};

// Descartes' basic emotions

const std::vector<std::string> emotions = {"wonder", "love", "hatred", "desire", "joy", "sadness"};


////////////////////////////////////////////////////////////////////////////////
// Locations
////////////////////////////////////////////////////////////////////////////////

// Location bounding boxes to check for tweets

std::vector<std::string> locations = {"-122.75,36.8,-121.75,37.8", "-74,40,-73,41"};


////////////////////////////////////////////////////////////////////////////////
// Twitter streaming search query url
////////////////////////////////////////////////////////////////////////////////

// The basic streaming request url, which we add parameters to

const std::string streaming_request_base = 
  "http://stream.twitter.com/1/statuses/filter.json";

// Make the url string for the streaming request

std::string build_streaming_request()
{
  std::string request;
  request = streaming_request_base;
  request += "?track=";
  request += boost::join(emotions, ",");
  request += "&locations=";
  request += boost::join(locations, ",");
  return request;
}


////////////////////////////////////////////////////////////////////////////////
// Emotion counts
////////////////////////////////////////////////////////////////////////////////

// A map to keep track of emotion word occurrence counts

typedef std::map<std::string, unsigned int> emotion_map;

// The emotion with the higest count. Pointer to make write atomic
// NOTE: Non-atomic, written to by one thread, read by another, probably bad

std::string current_emotion;

// The emotion map. So we can reset it

emotion_map twitter_emotion_map;

// Create an emotion map based on the list of emotions we are using

emotion_map build_emotion_map()
{
  emotion_map map;
  BOOST_FOREACH(const std::string & emotion, emotions)
  {
    map[emotion] = 0;
  }
  return map;
}

// reset the main emotion map

void reset_twitter_emotion_map()
{
  for(emotion_map::iterator i = twitter_emotion_map.begin();
      i != twitter_emotion_map.end();
      ++i)
  {
    (*i).second = 0;
  }
}

// Scan the data for mention of emotions
// This needs making more flexible (e.g. check for happy & happiness)

void increment_emotions(emotion_map & emomap, const std::string & data)
{
  std::string lower_data(data);
  std::transform(lower_data.begin(), lower_data.end(), lower_data.begin(),
		 ::tolower);
  BOOST_FOREACH(const std::string & emotion, emotions)
  {
    if(lower_data.find(emotion) != std::string::npos)
    {
      emomap[emotion] = emomap[emotion] + 1;
    }
  }
}

// Print the emotion map to a stream

void dump_emotion_map(std::ostream & stream, emotion_map & emomap)
{
  BOOST_FOREACH(const std::string & emotion, emotions)
  {
    stream << emotion << ": " << emomap[emotion] << " ";
  }
  stream << std::endl;
}

// Get the highest emotion count name

void highest_emotion_count(emotion_map & emomap, std::string & emotion)
{
  int max_count = -1;
  std::string max_name;
  for(emotion_map::iterator i = emomap.begin(); i != emomap.end(); ++i)
  {
    if(static_cast<int>((*i).second) > max_count)
    {
      max_name = (*i).first; 
      max_count = (*i).second;
    }
  }
  emotion = max_name;
}

////////////////////////////////////////////////////////////////////////////////
// Twitter streaming search result processing
////////////////////////////////////////////////////////////////////////////////

// The function called by curl when data is received from Twitter

size_t curl_callback_fun(void * ptr, size_t size, size_t nmemb, void * userdata)
{
  size_t data_length = size * nmemb;
  std::string data(reinterpret_cast<char *>(ptr), data_length);
  //std::cout << data << std::endl;
  emotion_map & emomap = *reinterpret_cast<emotion_map *>(userdata);
  increment_emotions(emomap, data);
  dump_emotion_map(std::cout, emomap);
  highest_emotion_count(emomap, current_emotion);
  return data_length;
}


////////////////////////////////////////////////////////////////////////////////
// Main flow of control
////////////////////////////////////////////////////////////////////////////////

// The curl thread

pthread_t twitter_streaming_thread;

// Do everything

void * run_streaming_search(void * user_pass)
{
  ::curl_global_init(CURL_GLOBAL_ALL);
  CURL * curl = ::curl_easy_init();
  if(curl)
  {
    std::string request = build_streaming_request();
    std::cout << request << std::endl;
    ::curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    ::curl_easy_setopt(curl, CURLOPT_USERPWD, static_cast<char*>(user_pass));
    ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback_fun);
    // Enable keep alive
    ::curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 0);
    ::curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 0);
    twitter_emotion_map = build_emotion_map();
    ::curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitter_emotion_map);
    CURLcode result = ::curl_easy_perform(curl);
    std::cerr << result << std::endl;
    ::curl_easy_cleanup(curl);
    ::curl_global_cleanup();
  }
  ::pthread_exit(0);
}

void start_twitter_search(char * userpass)
{
  int result = ::pthread_create(&twitter_streaming_thread, NULL,
				run_streaming_search, userpass);
  if(result != 0)
  {
    std::cerr << "Nonzero result from pthread_create: " << result << std::endl;
    ::exit(-1);
  }
}
