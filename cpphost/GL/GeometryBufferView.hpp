/**********************************************************************
File name: GeometryBufferView.hpp
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
#ifndef _PYUNI_GL_GEOMETRY_BUFFER_VIEW_H
#define _PYUNI_GL_GEOMETRY_BUFFER_VIEW_H

#include <GL/glew.h>

#include "GeometryBuffer.hpp"

namespace PyUni {
namespace GL {

class GeometryBufferView;

typedef boost::shared_ptr<GeometryBufferView> GeometryBufferViewHandle;

class GeometryBufferView {
    public:
        class AttributeSlice;
        
        class AttributeView {
            public:
                AttributeView(GeometryBufferView *view,
                    const GLsizei attribOffset,
                    const GLsizei attribLength,
                    const GLsizei vertexSize);
            private:
                GeometryBufferView *_view;
                GLsizei _attribOffset, _attribLength, _vertexSize;
                AttributeSlice *_slice;
                GLsizei _vertexCount, _vertexLength;
                GLVertexFloat *_data;
            public:
                void get(GLVertexFloat *data);
                GLsizei getAttributeLength();
                GLsizei getLength();
                GLsizei getSize();
                AttributeSlice *slice(const GLsizei start,
                    const GLsizei stop, const GLsizei step = 1,
                    const GLsizei attribOffset = 0,
                    const GLsizei attribLength = -1);
                void set(const GLVertexFloat *data);

            friend class AttributeSlice;
        };
        class AttributeSlice {
            public:
                AttributeSlice(AttributeView *view);
            private:
                AttributeView *_view;
                GLsizei _start, _stop, _step, _attribOffset, _attribLength;
            protected:
                void setUp(const GLsizei start, const GLsizei stop, const GLsizei step,
                    const GLsizei attribOffset, const GLsizei attribLength);
            public:
                void get(GLVertexFloat *data);
                GLsizei getAttributeLength();
                GLsizei getLength();
                GLsizei getSize();
                void set(const GLVertexFloat *data);

            friend class AttributeView;
        };
    public:
        GeometryBufferView(const GeometryBufferHandle bufferHandle,
            VertexIndexListHandle indicies);
        virtual ~GeometryBufferView();
    private:
        GeometryBufferHandle _buffer;
        const VertexFormat *_bufferFormat;
        VertexIndexListHandle _indicies;
        BufferMap *_map;
    private:
        AttributeView
            *_position,
            *_colour,
            *_texCoord[BUFFER_TEX_COORD_COUNT],
            *_normal,
            *_vertexAttrib[BUFFER_VERTEX_ATTRIB_COUNT];
    private:
        AttributeView *newAttribView(
            const GLsizei attribOffset, const GLsizei attribLength,
            const GLsizei vertexSize);
    public:
        GeometryBufferHandle getHandle() { return _buffer; }
        GLsizei getLength() const { return _indicies->size(); }
    public:
        AttributeView *getPositionView() { return _position; };
        AttributeView *getColourView() { return _colour; };
        AttributeView *getTexCoordView(const unsigned int texCoordIndex);
        AttributeView *getNormalView() { return _normal; };
        AttributeView *getVertexAttribView(const unsigned int attribIndex);

    friend class AttributeView;
    friend class AttributeSlice;
};

}
}

#endif
