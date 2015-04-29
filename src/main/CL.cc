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

/** @file CL.cc
 * Implementation of the command-line parsing of the main routine
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>

#include <boost/cstdint.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "CL.hh"



namespace ncd
{
namespace main
{


CL::CL()
    : m_opt_desc(new po::options_description("Options"))
{
  // Declare the supported options.
  po::options_description opt_general("General Configuration");
  opt_general.add_options()
      (HELP.c_str(), "produce help message")
      (VERS.c_str(), "show the version")
      ;

  po::options_description opt_io("I/O Configuration");
  opt_io.add_options()
      (RESULTS_DIR.c_str(), po::value <std::string>()->default_value("./results"), "results directory.")
      (SOURCE_DIR.c_str(), po::value <std::string>()->default_value("./sources"), "source directory containing the files")
      ;

  po::options_description opt_can("NCD Configuration");
  opt_can.add_options()
      (NORMALISE.c_str(), po::value <bool>()->default_value(1), "normalise if the maximum distance > 1")
      ;

  m_opt_desc->add(opt_general);
  m_opt_desc->add(opt_io);
  m_opt_desc->add(opt_can);
}


int CL::parse(int argc, char *argv[], args_t &p_args)
{
  po::variables_map vm;

  po::store(po::parse_command_line(argc, argv, (*m_opt_desc.get())), vm);
  po::notify(vm);

  if (vm.count(HELP)) {
    std::cout << (*m_opt_desc.get()) << std::endl;
    return EXIT_FAILURE;
  }

  if (vm.count(VERS)) {
    std::cout << PACKAGE_NAME << " " << PACKAGE_VERSION << std::endl;
    std::cout << argv[0] << std::endl;
    return EXIT_FAILURE;
  }

  if (vm.count(RESULTS_DIR.c_str())) {
    p_args.results_dir = vm[RESULTS_DIR.c_str()].as <std::string>();
  }

  if (vm.count(SOURCE_DIR.c_str())) {
    p_args.source_dir = vm[SOURCE_DIR.c_str()].as <std::string>();
  }

  if (vm.count(NORMALISE.c_str())) {
    p_args.normalise = vm[NORMALISE.c_str()].as <bool>();
  }

  std::cout << argv[0] << " " << PACKAGE_VERSION << std::endl;
  std::cout << PACKAGE_NAME << std::endl;
  std::cout << p_args << std::endl;

  return EXIT_SUCCESS;
}


}
}
