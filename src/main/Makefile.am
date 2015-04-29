# Copyright (C) 2013, 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = ncd

ncd_SOURCES =                                                           \
	main.cc                                                         \
	CL.cc

ncd_CPPFLAGS =                                                          \
	$(OPENMP_CXXFLAGS)                                              \
	$(BOOST_CPPFLAGS)                                               \
	-I./includes

ncd_LDADD =                                                             \
	$(BOOST_FILESYSTEM_LIB)                                         \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                    \
	$(BOOST_SYSTEM_LIB)

ncd_LDFLAGS =                                                           \
	$(BOOST_LDFLAGS)

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in
