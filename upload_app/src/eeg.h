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

#include <string>

// These are the values from the eeg values to use
// We discard the error and use the others
// eeg_above is exclusive
#define EEG_LOW 1
#define EEG_ABOVE 8

void load_emotions(const std::string & name);

void update_eegs(const std::string & emotion, double now);
void draw_eegs();

#endif

