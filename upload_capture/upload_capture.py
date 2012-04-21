#!/usr/bin/env python

# capture.py - Capture someone's emotions and expressions to a series of files
# Copyright (C) 2011, 2012  Rob Myers rob@robmyers.org
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

import freenect
import os
import sys
import time

import capture_emotions
import capture_expressions
from emotions import EMOTIONS, SECONDS_TO_CAPTURE_EMOTION_FOR


################################################################################
# Configuration
################################################################################

# How many seconds of data to capture each emotion/expression for
SECONDS_TO_CAPTURE_FOR = 10

# The delay before reading emotion/expression data
SECONDS_TO_WAIT_BEFORE_CAPTURING = 2


################################################################################
# Emotion capturing
################################################################################

def capture_data(duration):
    """Capture EEG and Kinect data for the time specified"""
    frames = []
    chunks = []
    synapse_socket = capture_emotions.start_receiving_eeg_data()
    last_frame = time.time()
    finish = last_frame + duration
    while last_frame < finish:
        last_frame = capture_expressions.handleKinect(frames, last_frame,
                                                     time.time())
        chunks.append(capture_emotions.receive_eeg_data(synapse_socket))
    capture_emotions.stop_receiving_eeg_data(synapse_socket)
    return frames, capture_emotions.eeg_data_from_chunks(chunks)


def capture_one(emotion, duration):
    """Capture the emotion to tsv files in person_name/emotion"""
    while True:
        print "\aPlease start (pretending that you are) feeling %s" % emotion
        print "I am going to start capturing data in %s seconds" % \
            SECONDS_TO_WAIT_BEFORE_CAPTURING
        time.sleep(SECONDS_TO_WAIT_BEFORE_CAPTURING)
        frames_and_eeg_data = capture_data(duration)
        print "\aDone."
        print "Did you manage to hold the feeling the entire time? [y/n]"
        if raw_input().lower().strip() in ["y", "yes"]:
            break
        else:
            print "Trying again..."
    #TODO: Expose this less :-)
    freenect.sync_stop()
    return frames_and_eeg_data


def capture_all(person_name, emotions, duration):
    """Capture each emotion in turn"""
    print """I am going to prompt you to (pretend to) feel the following emotions for %s seconds each: %s""" % (duration, ', '.join(emotions))
    captures = {}
    for emotion in emotions:
        if not os.path.exists(os.path.join(person_name, emotion)):
            capture = capture_one(emotion, duration)
            captures[emotion] = capture
    return captures


def dump_all(person_name, emotions, captures):
    """Save each emotion to file. This takes ages."""
    print "Saving to file, this will take ages..."
    for emotion in emotions:
        person_emotion_path = os.path.join(person_name, emotion)
        if not os.path.exists(person_emotion_path):
            print "Saving %s..." % emotion
            os.mkdir(person_emotion_path)
            frames, eeg_data = captures[emotion]
            eeg_data.to_tsv_files(person_emotion_path)
            capture_expressions.dumpFrames(frames, person_emotion_path)


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
        print "Folder for %s exists. Adding any missing emotions." % person_name
    else:
        os.mkdir(person_name)
    captures = capture_all(person_name, EMOTIONS,
                           SECONDS_TO_CAPTURE_EMOTION_FOR)
    dump_all(person_name, EMOTIONS, captures)


if __name__ == "__main__":
    main()

