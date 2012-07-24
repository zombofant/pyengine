/**********************************************************************
File name: BufferMap.hpp
This file is part of: Pythonic Universe

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYUNI_UTILS_BUFFER_MAP_H
#define _PYUNI_UTILS_BUFFER_MAP_H

#include <cstddef>

#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace GL {

class BufferMap {
    public:
        BufferMap():
            _offset(0) {};
        virtual ~BufferMap() {};
    private:
        size_t _offset;
    public:
        size_t getOffset() const { return _offset; }
        void setOffset(const size_t offset) { _offset = offset; }
    public:
        virtual size_t map(const size_t index) { return index+_offset; }; 
};

typedef boost::shared_ptr<BufferMap> BufferMapHandle;

}
}

#endif
