// Copyright (C) 2013, 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
namespace fs = boost::filesystem;

#include <boost/tuple/tuple.hpp>

#include "CL.hh"
namespace cmain = ncd::main;

#include "NCD.hh"

typedef std::pair<std::string, boost::uint32_t> val;
typedef boost::unordered_map<std::string, val> map;
typedef std::vector<boost::tuple<std::string, std::string, double> > distVec;


int main(int argc, char *argv[])
{
  map m_map;

  std::ios_base::sync_with_stdio(false);
  cmain::args_t args;
  cmain::CL cl;

  if (cl.parse(argc, argv, args)) {
    return EXIT_SUCCESS;
  }

  // read the files into memory
  fs::path sourceDir = fs::path(args.source_dir);
  if(!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
    std::cerr << "Error: The source directory " << args.source_dir << " needs to exist!" << std::endl;
    return EXIT_FAILURE;
  } else {
    fs::recursive_directory_iterator it(sourceDir);
    fs::recursive_directory_iterator endit;

    while(it != endit) {
      std::ifstream in;
      in.open(it->path().c_str());
      if (!in.is_open()) {
        std::cerr << "Error: Could not open file " << it->path() << std::endl;
        return EXIT_FAILURE;
      } else {
        m_map[it->path().stem().string()] = std::pair<std::string, boost::uint32_t> (
            static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str(), 0);
        in.close();
      }
      ++it;
    }
  }

  // compute the compressed number of bytes for each file in the map
  #pragma omp parallel for shared(m_map) default(none)
  for (boost::uint32_t i = 0; i < m_map.size(); ++i) {
    map::iterator h_i = m_map.begin();
    std::advance(h_i, i);
    boost::uint32_t c = ncd::compressStr((h_i->second).first.c_str(), (h_i->second).first.length());

    #pragma omp critical
    {
      (h_i->second).second = c;
#ifndef NDEBUG
      std::cout << h_i->first << "," << (h_i->second).first.length() << "," << (h_i->second).second << std::endl;
#endif
    }
  }

  // store the distances in a vector
  distVec dists;
  double maxDist = 1.0;

  // compute all distances
  for (boost::uint32_t j = 0; j < m_map.size(); ++j) {
    map::const_iterator it = m_map.begin();
    std::advance(it, j);

    #pragma omp parallel for shared(m_map, std::cout, it, j, dists, maxDist) default(none)
    for (boost::uint32_t i = j+1; i < m_map.size(); ++i) {
      map::const_iterator h_it = m_map.begin();
      std::advance(h_it, i);
      double dist = ncd::NCD<val, double>()(h_it->second, it->second);
      #pragma omp critical
      {
        if (dist > maxDist) {
          maxDist = dist;
        }
        dists.push_back(boost::tuple<std::string, std::string, double>(it->first, h_it->first, dist));
#ifndef NDEBUG
        std::cout << h_it->first << "," << it->first << "," << dist << std::endl;
#endif
      }
    }
  }

  // serialise the similarity matrix
  std::ofstream out;
  std::string outFile = args.results_dir + "/sim.csv";
  out.open(outFile.c_str());
  distVec::iterator it_end = dists.end();
  for (distVec::iterator it = dists.begin(); it != it_end; ++it) {
    double dist = it->get<2>();
    if (args.normalise) {
      dist /= maxDist;
    }
    out << it->get<0>() << "," << it->get<1>() << "," << dist << std::endl;
  }
  out.close();

  return EXIT_SUCCESS;
}
