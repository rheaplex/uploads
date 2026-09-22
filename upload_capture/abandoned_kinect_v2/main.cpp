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

// make && LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${HOME}/freenect2/lib" ./capture nom5

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>

#include "emotions.h"


const size_t FRAME_TIMEOUT = 10 * 1000;

static const std::string xyz_bin_extension = ".xyz.bin";
static const std::string uv_bin_extension = ".uv.bin";


////////////////////////////////////////////////////////////////////////
// Filesystem
////////////////////////////////////////////////////////////////////////

bool ensureOutputDirectory (const std::string & personName) {
    bool result = true;
    boost::filesystem::path personPath = personName; 
    if (! boost::filesystem::exists(personPath)) {
        result = boost::filesystem::create_directory(personPath);
    } else {
        std::cout << "Folder for " << personName
                  << " exists. Adding any missing emotions."
                  << std::endl;
    }
    return result;
}

boost::filesystem::path emotionPath(const std::string & personName,
                                    const std::string & emotion) {
    return boost::filesystem::path(personName)
        / boost::filesystem::path(emotion);
}

bool emotionPathExists (const std::string & personName,
                        const std::string & emotion) {
    return boost::filesystem::exists(emotionPath(personName,
                                                 emotion));
}

bool createEmotionPath (const std::string & personName,
                        const std::string & emotion) {
    return boost::filesystem::create_directory(emotionPath(personName,
                                                           emotion));
}

boost::filesystem::path framePath(const std::string & personName,
                                  const std::string & emotion,
                                  const unsigned int when,
                                  const std::string & extension) {
    return emotionPath(personName, emotion)
        / boost::filesystem::path(std::to_string(when) + extension);
}


////////////////////////////////////////////////////////////////////////
// Kinect interaction
////////////////////////////////////////////////////////////////////////

bool initKinect (
    libfreenect2::Freenect2Device *&dev,
    libfreenect2::SyncMultiFrameListener *&listener
) {
    libfreenect2::Freenect2 freenect2;
    //FIXME: allow user to specify from command line.
    libfreenect2::PacketPipeline *pipeline
        = new libfreenect2::CpuPacketPipeline();
    // = new libfreenect2::OpenGLPacketPipeline();
    // = new libfreenect2::OpenCLKdePacketPipeline(deviceId);
    // = new libfreenect2::CudaKdePacketPipeline(deviceId);
    if(freenect2.enumerateDevices() == 0)
    {
        std::cout << "No device connected!" << std::endl;
        return false;
    }
    //FIXME: allow user to specify from command line
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    dev = freenect2.openDevice(serial, pipeline);
    if(dev == NULL)
    {
        std::cout << "failure opening device!" << std::endl;
        return false;
    }
    listener = new libfreenect2::SyncMultiFrameListener(
        libfreenect2::Frame::Color
        | libfreenect2::Frame::Depth
    );
    dev->setColorFrameListener(listener);
    dev->setIrAndDepthFrameListener(listener);
    if (!dev->start()) {
        return false;
    }
    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    return true;
}

// Use frames rather than time so each sample has the same number of frames

bool loopKinect (
    libfreenect2::Freenect2Device *dev,
    libfreenect2::SyncMultiFrameListener *listener,
    const unsigned int numFrames
) {
    libfreenect2::FrameMap frames;
    libfreenect2::Registration* registration = new libfreenect2::Registration(
        dev->getIrCameraParams(),
        dev->getColorCameraParams()
    );
    libfreenect2::Frame undistorted(512, 424, 4);
    libfreenect2::Frame registered(512, 424, 4);
    unsigned int frameCount = 0;
    while (frameCount < numFrames) {
        // Get the frame data
        if (!listener->waitForNewFrame(frames, FRAME_TIMEOUT)) {
            std::cerr << "timeout!" << std::endl;
            delete registration;
            return false;
        }
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        registration->apply(rgb, depth, &undistorted, &registered);
        // Copy the frame data
        size_t rgbSize = rgb->width * rgb->height * rgb->bytes_per_pixel;
        unsigned char * rgbCopy = new unsigned char[rgbSize];
        if (rgb->format == ) {
            for
        } else {
            for
        }
            
        std::copy(rgb->data, rgb->data + rgbSize, rgbCopy);
        size_t depthSize = depth->width * depth->height;
        float * depthCopy = new float[depthSize];
        for (size_t i = 0; i < depthSize; i++) {
            depthCopy[i] = *reinterpret_cast<float*>(&depth->data[i * 4]);
        }

        //FIXME: Process the frame data        

        listener->release(frames);
        frameCount++;
    }
    delete registration;
    return true;
}

void shutdownKinect (libfreenect2::Freenect2Device *&dev) {
    dev->stop();
    dev->close();
    dev = NULL;
}


////////////////////////////////////////////////////////////////////////
// Capture
////////////////////////////////////////////////////////////////////////

typedef struct Frame {
    unsigned int when;
    unsigned char * rgb;
    size_t rgbSize;
    unsigned char * depth;
    size_t depthSize;
} Frame;

typedef struct Capture {
    std::vector<Frame> frames;
} Capture;

typedef std::map<std::string, Capture> Captures;

void dumpRGB (const std::string & filepath) {
    boost::gil::rgb8_image_t imgRGB;
    boost::gil::png_write_view(filepath, boost::gil::view(imgRGB));
}

void dumpXYZ(const std::string & filepath,
             boost::shared_array<float> & floats,
             size_t length) {
    std::ofstream file(filepath, std::ofstream::binary);
    for(size_t i = 0; i < length; i++) {
        file.write(reinterpret_cast<const char*>(&(floats[i])), sizeof(float));
    }
    file.close();
}

void dumpFrame (const Frame & frame, const std::string & personName,
                const std::string & emotion) {
    dumpLeFloats(path_root.string() + ".xyz.bin", Frame.xyz, num_coords * 3);
    dumpLeFloats(path_root.string() + ".uv.bin", Frame.uv, other_num_coords * 2);
}

void printInitialPrompt () {
    std::cout << "I am going to prompt you to (pretend to) feel"
              << " the following emotions for "
              << SECONDS_TO_CAPTURE_EMOTION_FOR << " seconds:" << std::endl
              << boost::algorithm::join(EMOTIONS, ", ") << "." << std::endl;
}

Capture captureOne (const std::string & emotion) {
    Capture capture;
    for (;;) {
        std::cout << "\a"
                  << "Please start (pretending that you are) feeling "
                  << emotion << std::endl;
        std::cout << "I am going to start capturing data in "
                  << SECONDS_TO_WAIT_BEFORE_CAPTURING
                  << " seconds..." << std::endl;
        sleep(SECONDS_TO_WAIT_BEFORE_CAPTURING);
        //bool result = captureData(capture, emotion);
        std::cout << "\a" << "Done." << std::endl;
        std::cout << "Did you manage to hold the feeling the entire time? [y/n]"
                  << std::endl;
        char answer;
        std::cin >> answer;
        if(answer == 'y' || answer == 'Y') {
            break;
        } else {
            std::cout << "Trying again..." << std::endl;
        }
    }
    return capture;
}

bool captureAll (const std::string & personName, Captures & captures) {
    bool status = false;
    printInitialPrompt();
    libfreenect2::Freenect2Device *dev = NULL;
    libfreenect2::SyncMultiFrameListener *listener = NULL;
    bool connected = initKinect(dev, listener);
    if (connected) {
        for (const std::string & emotion : EMOTIONS) {
            if (! emotionPathExists(personName, emotion)) {
                captures[emotion] = captureOne(emotion);
            } else {
                std::cout << "Already captured "
                          << emotion << ", skipping." << std::endl;
            }
        }
        shutdownKinect(dev);
        status = true;
    } else {
        std::cerr << "No Kinect found (or error connecting)!" << std::endl;
    }
    return status;
}


////////////////////////////////////////////////////////////////////////
// Dumping
////////////////////////////////////////////////////////////////////////

bool dumpAll(const std::string & personName, Captures captures) {
    std::cout << "Saving to file, this will take ages..." << std::endl;
    for (const std::string & emotion : EMOTIONS) {
        if (! emotionPathExists(personName, emotion)) {
            std::cout << "Saving " << emotion << "..." << std::endl;
            bool created = createEmotionPath(personName, emotion);
            if (! created) {
                //TODO: handle error
            }
        }
    }
    std::cout <<"Done." << std::endl;
    return true;
}


////////////////////////////////////////////////////////////////////////
// Main flow of execution
////////////////////////////////////////////////////////////////////////

void printUsage (char * argv[]) {
    std::cerr << "USAGE: " << argv[0] << " PERSON_NAME" << std::endl;
}

int main (int argc, char * argv[]) {
    if (argc != 2) {
        printUsage(argv);
    } else {
        std::cout << "Uploads emotion capture v2." << std::endl;
        const std::string personName = argv[1];
        bool result = ensureOutputDirectory(personName);
        if (result) {
            Captures captures;
            bool result = captureAll(personName, captures);
            if (result) {
                if (captures.size() > 0) {
                    bool result = dumpAll(personName, captures);
                    if (result) {
                        std::cout << "OK!" << std::endl;
                    } else {
                        std::cout << "Error dumping emotions to file."
                                  << std::endl;
                    }
                } else {
                    std::cout << std::endl << "All emotions already captured."
                              << std::endl;
                }
            } else {
                std::cerr << "Couldn't capture emotions" << std:: endl;
            }
        } else {
            std::cerr << "Couldn't create output directory" << std::endl;
        }
    }
    return 0;
}
