////////////////////////////////////////////////////////////////////////////////
//    expression.h - load serialized kinect frame data
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


#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

void expression_add_options(po::options_description & desc);
void expression_initialize(const po::variables_map & vm);
void load_expressions(const std::string & person_dir);
void update_expression(const std::string & emotion, double now);
void draw_expression();

#endif
