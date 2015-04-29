// Copyright (C) 2013, 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
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

/** @file NCD.hh
 * Declaration of the compression based distance measure
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __NCD_HH__
#define __NCD_HH__

#include <algorithm>
#include <functional>

#include <boost/type_traits.hpp>

#include "zlib.h"


namespace ncd
{

#define CHUNK 16384

boost::uint32_t compressStr(const char *in_data, size_t in_data_size)
{
    const size_t BUFSIZE = 128 * 1024;
    boost::uint32_t compressionLength = 0;
    boost::uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.next_in = reinterpret_cast<boost::uint8_t *>(const_cast<boost::remove_const<const char>::type *>(in_data));
    strm.avail_in = in_data_size;
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    deflateInit(&strm, Z_DEFAULT_COMPRESSION);

    while (strm.avail_in != 0) {
        int res = deflate(&strm, Z_NO_FLUSH);
        assert(res == Z_OK);
        if (strm.avail_out == 0) {
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
            compressionLength += BUFSIZE;
        }
    }

    int deflate_res = Z_OK;
    while (deflate_res == Z_OK) {
        if (strm.avail_out == 0) {
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
            compressionLength += BUFSIZE;
        }
        deflate_res = deflate(&strm, Z_FINISH);
    }

    compressionLength += BUFSIZE - strm.avail_out;
    assert(deflate_res == Z_STREAM_END);
    deflateEnd(&strm);

    return compressionLength;
}

/**
 * Calculate the NCD distance, where T is a set
 */
template <class T, class R> struct NCD : std::binary_function <T, T, R>
{
    R operator() (const T& x, const T& y) const {
        std::string xy = x.first + " " + y.first;
        boost::uint32_t c_xy = ncd::compressStr(xy.c_str(), xy.length());
#ifndef NDEBUG
        std::cout << "(" << c_xy << " - " << std::min(x.second, y.second) << ")/" << std::max(x.second, y.second) << std::endl;
#endif
        return (boost::lexical_cast<double>(c_xy - std::min(x.second, y.second)) / boost::lexical_cast<double>(std::max(x.second, y.second)));
    }
};

}


#endif
