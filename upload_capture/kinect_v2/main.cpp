// main.cpp - Main flow of execution.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>

#include "emotions.h"


////////////////////////////////////////////////////////////////////////
// Filesystem
////////////////////////////////////////////////////////////////////////

int ensureOutputDirectory (const char * personName) {
    int result = 0;
    struct stat info;
    if (stat(personName, &info) != 0 ) {
        result = mkdir(personName, S_IRWXU);
    } else if(info.st_mode & S_IFDIR) {
        printf("Folder for %s exists. Adding any missing emotions.\n",
               personName);
    } else {
        printf("Not a directory: %s", personName);
        result = ENOTDIR;
    }
    return result;
}

// Caller must free

char * emotionPath(const char * personName, const char * emotion) {
    // +2 for / and \0
    char * path = (char *)malloc(strlen(personName) + strlen(emotion) + 2);
    sprintf(path, "%s/%s", personName, emotion);
    return path;
}

bool emotionPathExists (const char * personName, const char * emotion) {
    bool exists = true;
    char * path = emotionPath(personName, emotion);
    struct stat info;
    // If the path doesn't exist, make it
    if (stat(path, &info) != 0 ) {
        exists = false;
    }
    // Ignore weirder 
    free(path);
    return exists;
}

int createEmotionPath (const char * personName, const char * emotion) {
    char * path = emotionPath(personName, emotion);
    int result = mkdir(path, S_IRWXU);
    free(path);
    return result;
}


////////////////////////////////////////////////////////////////////////
// Capture
////////////////////////////////////////////////////////////////////////

typedef struct Capture {

} Capture;

void printInitialPrompt () {
    printf("I am going to prompt you to (pretend to) feel the following emotions for %i seconds each:\n   ",
           SECONDS_TO_CAPTURE_EMOTION_FOR);
    // Array join...
    for (unsigned int i = 0; i < NUM_EMOTIONS; i++) {
        printf(" %s", EMOTIONS[i]);
        if (i < (NUM_EMOTIONS - 1)) {
            printf(",");
        }
    };
    printf(".\n");
}

Capture * captureOne (const char * emotion) {
    Capture * capture = (Capture*)malloc(sizeof(Capture));
    while (1) {
        printf("\a\nPlease start (pretending that you are) feeling %s\n",
               emotion);
        printf("I am going to start capturing data in %i seconds...\n",
               SECONDS_TO_WAIT_BEFORE_CAPTURING);
        sleep(SECONDS_TO_WAIT_BEFORE_CAPTURING);
        //captureData(capture);
        printf("\aDone.\n");
        printf("Did you manage to hold the feeling the entire time? [y/n]\n");
        char answer;
        // space in the spec to ignore leading space in input
        scanf(" %c", &answer);
        if(answer == 'y' || answer == 'Y') {
            break;
        } else {
            printf("\nTrying again...\n");
        }
    }
    return capture;
}

Capture ** captureAll (const char * personName) {
    Capture ** captures = (Capture**)malloc(sizeof(Capture *) * NUM_EMOTIONS);
    printInitialPrompt();
    for (unsigned int i = 0; i < NUM_EMOTIONS; i++) {
        const char * emotion = EMOTIONS[i];
        if (! emotionPathExists(personName, emotion)) {
            captures[i] = captureOne(emotion);
        } else {
            printf("\nAlready captured %s, skipping.\n", emotion);
        }
    }
    return captures;
}


////////////////////////////////////////////////////////////////////////
// Dumping
////////////////////////////////////////////////////////////////////////

int dumpAll(const char * personName, Capture ** captures) {
    printf("Saving to file, this will take ages...\n");
    for (unsigned int i = 0; i < NUM_EMOTIONS; i++) {
        const char * emotion = EMOTIONS[i];
        if (! emotionPathExists(personName, emotion)) {
            printf("Saving %s...\n", emotion);
            int err = createEmotionPath(personName, emotion);
            //TODO: handle error
        }
    }
    printf("Done.\n");
    return 0;
}


////////////////////////////////////////////////////////////////////////
// Main flow of execution
////////////////////////////////////////////////////////////////////////

void printUsage (char * argv[]) {
    printf("USAGE: %s PERSON_NAME\n", argv[0]);
}

int main (int argc, char * argv[]) {
    int result = 0;
    if (argc != 2) {
        printUsage(argv);
    } else {
        const char * personName = argv[1];
        result = ensureOutputDirectory(personName);
        if (result == 0) {
            Capture ** captures = captureAll(personName);
            dumpAll(personName, captures);
        }
    }
    return 0;
}
