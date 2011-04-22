#!/usr/bin/env python

# capture_emotions.py - Capture someone's emotions to a series of files
# Copyright (C) 2011  Rob Myers rob@robmyers.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or 
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


################################################################################
# Imports
################################################################################


import os
import simplejson
import socket
import sys
import time


################################################################################
# Configuration
################################################################################

# How many seconds of data to capture each emotion for
SECONDS_TO_CAPTURE_EMOTION_FOR = 10

# The delay before reading emotion data
SECONDS_TO_WAIT_BEFORE_CAPTURING = 2

# The emotions to capture. For more information, see:
# http://en.wikipedia.org/wiki/Emotion_classification#Basic_and_Complex_Emotions
EKMAN = ["anger", "disgust", "fear", "happiness", "sadness", "surprise"]
DESCARTES = ["wonder", "love", "hatred", "desire", "joy", "sadness"]
EMOTIONS = DESCARTES

# The amount of data to read each time from the Synaptic server
CHUNK_MAX_SIZE = 256


################################################################################
# Eeg Data object
################################################################################


class EegData(object):
    POWER_LEVELS_FILE = "power_levels.txt"
    RAW_EEG_FILE = "raw_eeg.txt"

    def __init__(self):
        self.raw_eeg = []
        self.power_levels = []

    def add_raw_eeg(self, timestamp, level):
        """Add a rawEeg state update"""
        self.raw_eeg.append((timestamp, level))

    def print_raw_eeg(self, outfile, separator='\t', header=True):
        """Print the raw eeg data"""
        if header:
            print >>outfile, "#%s" % separator.join(("timestamp", "rawEeg"))
        for eeg in self.raw_eeg:
            print >>outfile, separator.join([str(level) for level in eeg])

    def add_power_levels(self, timestamp, poorSignalLevel, lowAlpha, highAlpha,
                         lowBeta, highBeta, lowGamma, highGamma, 
                         attention, meditation):
        """Add an eegPower state update"""
        self.power_levels.append((timestamp, poorSignalLevel, lowAlpha,
                                  highAlpha, lowBeta, highBeta, lowGamma,
                                  highGamma, attention, meditation))

    def print_power_levels(self, outfile, separator='\t', header=True):
        """Print the power levels"""
        if header:
            print >>outfile, "#%s" % separator.join(("timestamp", 
                                                     "poorSignalLevel",
                                                     "lowAlpha", "highAlpha",
                                                     "lowBeta", "highBeta",
                                                     "lowGamma", "highGamma",
                                                     "attention", "meditation"))
        for levels in self.power_levels:
            print >>outfile, separator.join([str(level) for level in levels])

    def populate_from_data(self, data_lines):
        """Populate this object from a list of lines of data messages"""
        poorSigLev = None
        eegPow = None
        attention = None
        for line in data_lines:
            message = simplejson.loads(line)
            print message
            if "rawEeg" in message:
                self.add_raw_eeg(message["timestamp"], message["rawEeg"])
            if "poorSignalLevel" in message:
                poorSigLev = message
            if "eegPower" in message:
                eegPow = message
            if "eSense" in message:
                e_sense = message["eSense"]
                if "attention" in e_sense:
                    attention = message
                # This assumes meditation always comes last
                elif "meditation" in e_sense:
                    # If we've cached all the data we need to store power levels
                    if poorSigLev and eegPow and attention:
                        # Make sure our time-sequencing-based assumptions hold
                        assert(poorSigLev["timestamp"] ==
                               eegPow["timestamp"] ==
                               attention["timestamp"] ==
                               message["timestamp"])
                        self.add_power_levels(message["timestamp"],
                                              poorSigLev["poorSignalLevel"],
                                              eegPow["eegPower"]["lowAlpha"], 
                                              eegPow["eegPower"]["highAlpha"], 
                                              eegPow["eegPower"]["lowBeta"],
                                              eegPow["eegPower"]["highBeta"], 
                                              eegPow["eegPower"]["lowGamma"],
                                              eegPow["eegPower"]["highGamma"],
                                              attention["eSense"]["attention"],
                                              e_sense["meditation"])
                    # Always reset the cached messages at this point as we may
                    # have reached meditation near the start of the data
                    # without having received the other data first
                    poorSigLev = None
                    eegPow = None
                    attention = None

    def to_tsv_files(self, enclosing_folder):
        """Convert this object's fields to several tsv files in the
           enclosing folder"""
        # Save raw eeg
        raw_eeg_filename = os.path.join(enclosing_folder, EegData.RAW_EEG_FILE)
        raw_eeg = open(raw_eeg_filename, 'w')
        self.print_raw_eeg(raw_eeg)
        raw_eeg.close()
        # Save power levels
        levels_filename = os.path.join(enclosing_folder,
                                       EegData.POWER_LEVELS_FILE)
        levels = open(levels_filename, 'w')
        self.print_power_levels(levels)
        levels.close()


################################################################################
# Synaptic server data processing
################################################################################


def get_data_for_n_seconds(seconds):
    """Read data from the Synapse server for n seconds,
       and return it as a single string"""
    end_time = time.time() + seconds
    s = socket.create_connection(("localhost", 13854))
    f = s.makefile()
    chunks = []
    while time.time() < end_time:
        chunks.append(s.recv(CHUNK_MAX_SIZE))
    s.close()
    return ''.join(chunks)


def split_data(output):
    """Split the data into individual entries"""
    splitted = output.split('\r')
    # If the last line is empty or incomplete, strip it
    if splitted[-1].strip == '' or not splitted[-1].endswith('}'):
        splitted = splitted[:-1]
    return splitted


def eeg_data_from_n_seconds_data(seconds):
    """Capture n seconds data from the Synapse server and return as an
       EegData object"""
    raw_data = get_data_for_n_seconds(2)
    data_lines = split_data(raw_data)
    eeg_data = EegData()
    eeg_data.populate_from_data(data_lines)
    return eeg_data


################################################################################
# Emotion capturing
################################################################################


def capture_emotion(person_name, emotion, duration):
    """Capture the emotion to tsv files in person_name/emotion"""
    while True:
        print "OK, I am going to start capturing data in %s seconds" % \
            SECONDS_TO_WAIT_BEFORE_CAPTURING
        print "Please start [pretending that you are] feeling %s" % emotion
        time.sleep(SECONDS_TO_WAIT_BEFORE_CAPTURING)
        eeg_data = eeg_data_from_n_seconds_data(duration)
        print "Done. Do you feel that was a good one? [y/n]"
        if raw_input().lower().strip() in ["y", "yes"]:
            print "Saving to file..."
            person_emotion_path = os.path.join(person_name, emotion)
            os.mkdir(person_emotion_path)
            eeg_data.to_tsv_files(person_emotion_path)
            break
        else:
            print "Trying again..."


def capture_emotions(person_name, emotions, duration):
    """Capture each emotion in turn"""
    print """I am going to prompt you to pretend to feel the following emotions for %s seconds each: %s""" % (duration, ', '.join(emotions))
    for emotion in emotions:
            capture_emotion(person_name, emotion, duration)


################################################################################
# Main flow of execution
################################################################################


def usage():
    """Print usage instructions"""
    print "USAGE: %s person_name" % sys.argv[0]
    sys.exit(1)


def main():
    """Capture emotions."""
    if len(sys.argv) != 2:
        usage()
    person_name = sys.argv[1]
    if os.path.exists(person_name):
        print "Folder of emotions for %s already exists. Exiting." % person_name
        sys.exit(1)
    else:
        os.mkdir(person_name)
    capture_emotions(person_name, EMOTIONS, SECONDS_TO_CAPTURE_EMOTION_FOR)


if __name__ == "__main__":
    main()
