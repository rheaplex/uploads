////////////////////////////////////////////////////////////////////////////////
//    convert_format.cpp - Convert gzipped csv to raw little-endian floats.
//    Copyright (C) 2019 Rhea Myers <rhea@myers.studio>
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


// Much copy-and-paste antipatterning within.


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/regex.hpp>
#include <boost/shared_array.hpp>
#include <boost/tokenizer.hpp>


////////////////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////////////////

// Descartes' basic emotions

const std::vector<std::string> emotions = {
    "wonder", "love", "hatred", "desire", "joy", "sadness"
};

// File extensions
static const std::string xyz_extension = ".xyz.gz";
static const std::string uv_extension = ".uv.gz";
static const std::string xyz_bin_extension = ".xyz.bin";
static const std::string uv_bin_extension = ".uv.bin";


////////////////////////////////////////////////////////////////////////////////
// File handling
////////////////////////////////////////////////////////////////////////////////

// Load the lines of a gzipped text file into a vector of strings

void slurp_gzipped_lines(const std::string & path,
                         std::vector<std::string> & lines) {
    std::ifstream file(path);//, ios_base::in | ios_base::binary);
    if(! file) {
        throw std::runtime_error("Problem opening file: " + path);
    }
    boost::iostreams::filtering_stream<boost::iostreams::input> in;
    in.push(boost::iostreams::gzip_decompressor());
    in.push(file);
    std::string line;
    while(std::getline(in, line)){
        lines.push_back(line);
    }
}

// Load the values of a gzipped tsv file into an array of floats

size_t slurp_gzipped_csv_floats(boost::shared_array<float> & values,
                                const std::string & path,
                                int stride) {
    std::vector<std::string> lines;
    slurp_gzipped_lines(path, lines);
    values = boost::shared_array<float>(new float[lines.size() * stride]);
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t");
    for(size_t i = 0; i < lines.size(); i++){
        tokenizer::iterator tokens = tokenizer(lines[i], sep).begin();
        int index = i * stride;
        for(int j = 0; j < stride; j++){
            values[index + j] = std::atof((*tokens).c_str());
            tokens++;
        }
    }
    return lines.size();
}

// Write the array of floats as raw little endian to file

void dump_le_floats(const std::string & filepath,
                    boost::shared_array<float> & floats,
                    size_t length) {
    std::ofstream file(filepath, std::ofstream::binary);
    for(size_t i = 0; i < length; i++) {
        file.write(reinterpret_cast<const char*>(&(floats[i])), sizeof(float));
    }
    file.close();
}


////////////////////////////////////////////////////////////////////////////////
// Expression conversion
////////////////////////////////////////////////////////////////////////////////

// Predicate to check whether a file path is a png file (or at least ends .png)

bool is_png_file(const boost::filesystem::directory_iterator & i){
  static boost::regex png_filter( ".*\\.png" );
  bool is = true;
  if(! boost::filesystem::is_regular_file(i->status())){
    is = false;
  }
  boost::smatch what;
  if(! boost::regex_match(i->path().filename().string(), what, png_filter)){
      is = false;
  }
  return is;
}

// Convert a frame from csv to raw little-endian floats

void convert_frame(const boost::filesystem::path & path_root,
                   double when_base) {
    // The path is of the format /a/b/c/2346.12
    const double when = std::atof(path_root.filename().c_str()) - when_base;
    boost::shared_array<float> xyz;
    size_t num_coords = slurp_gzipped_csv_floats(xyz,
                                                 path_root.string() 
                                                 + xyz_extension,
                                                 3);
    boost::shared_array<float> uv;
    size_t other_num_coords = slurp_gzipped_csv_floats(uv,
                                                       path_root.string()
                                                       + uv_extension,
                                                       2);
    if(num_coords != other_num_coords){
        throw std::runtime_error("Coord count mismatch");
    }
    dump_le_floats(path_root.string() + ".xyz.bin", xyz, num_coords * 3);
    dump_le_floats(path_root.string() + ".uv.bin", uv, other_num_coords * 2);
}

// Load all the frames for an emotion directory into a vector of Frame objects

void convert_expression(const std::string & emotion_dir) {
    std::cerr << emotion_dir << std::endl;
    boost::filesystem::directory_iterator end;
    boost::filesystem::directory_iterator i(emotion_dir);
    double when_base = -1.0;
    unsigned int loaded_count = 0;
    // Load each frame
    for(; i != end; ++i ){
        // use .pngs as the indicators of times. alphabetically they're first
        if (is_png_file(i)){
            // Strip the .png extention to get a base /path/time.stamp
            boost::filesystem::path path(i->path());
            path.replace_extension("");
            std::cerr << path.string() << std::endl;
            // We need to set the base timestamp from the first png filename
            if(when_base == -1.0){
                when_base = std::atof(path.filename().c_str());
            }
            convert_frame(path, when_base);
        }
    }
}

// Convert expression frames.
// Each frame on disk has the name format "seconds.fraction.png",
// with.xyz and .uv files accompanying it

void convert_expressions(const std::string & person_dir) {
    std::vector<std::string>::const_iterator end = emotions.end();
    for (std::vector<std::string>::const_iterator i = emotions.begin();
         i != end; ++i){
        convert_expression(person_dir + "/" + *i);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Main flow of execution
////////////////////////////////////////////////////////////////////////////////

int main (const int argc, char * argv[]) {
    if (argc != 2) {
        std::cerr
            << "Usage: "
            << argv[0]
            << " <full path to directory containing emotion directories>"
            << std::endl;
        exit(1);
    }
    const std::string person_dir = std::string(argv[1]);
    convert_expressions(person_dir);
    return 0;
}
