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

// We use c++0x, which needs specifying:
// g++ -std=c++0x -lcurl -o twtstream twitter_streaming_search.cpp


////////////////////////////////////////////////////////////////////////////////
// includes
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

#include <curl/curl.h>


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
  return data_length;
}


////////////////////////////////////////////////////////////////////////////////
// Main flow of control
////////////////////////////////////////////////////////////////////////////////

// Until we get a command line option processor done, just check simply

void check_args(const int argc, const char * argv[])
{
  if(argc != 2)
  {
    std::cerr << "USAGE: Pass Twitter username:password as first argument"
	      << std::endl;
    std::exit(1);
  }
}

// Do everything

int main(const int argc, const char * argv[])
{
  check_args(argc, argv);
  const char * user_pass = argv[1];
  curl_global_init(CURL_GLOBAL_ALL);
  CURL * curl = curl_easy_init();
  if(curl)
  {
    std::string request = build_streaming_request();
    std::cout << request << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pass);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback_fun);
    emotion_map map = build_emotion_map();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &map);
    CURLcode result = curl_easy_perform(curl);
    std::cerr << result << std::endl;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  }
  return 0;
}
