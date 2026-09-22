// emotions.cpp - Emotion categories/names.
// Copyright (C) 2011,2020 Rhea Myers rhea@myers.studio
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include <vector>

#include "emotions.h"

// The emotions to capture. For more information, see:
// https://en.wikipedia.org/wiki/Emotion_classification//Basic_and_Complex_Emotions
const std::vector<std::string> EMOTIONS = {
// Ekman
// "anger", "disgust", "fear", "happiness", "sadness", "surprise"
// Descartes
    "wonder", "love", "hatred", "desire", "joy", "sadness"
};

const unsigned int NUM_EMOTIONS = EMOTIONS.size();

// How many seconds of data to capture each emotion for
const unsigned int SECONDS_TO_CAPTURE_EMOTION_FOR = 10;

// The delay before reading emotion data
const unsigned int SECONDS_TO_WAIT_BEFORE_CAPTURING = 2;
