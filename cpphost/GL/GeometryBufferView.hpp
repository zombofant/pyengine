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
                GLsizei getLength();
                GLsizei getSize();
                AttributeSlice *slice(const GLsizei start, const GLsizei stop, const GLsizei step = 1);
                void set(const GLVertexFloat *data);

            friend class AttributeSlice;
        };
        class AttributeSlice {
            public:
                AttributeSlice(AttributeView *view);
            private:
                AttributeView *_view;
                GLsizei _start, _stop, _step;
            protected:
                void setUp(const GLsizei start, const GLsizei stop, const GLsizei step = 1);
            public:
                void get(GLVertexFloat *data);
                GLsizei getLength();
                GLsizei getSize();
                void set(const GLVertexFloat *data);

            friend class AttributeView;
        };
    private:
        GeometryBufferView(const GenericGeometryBufferHandle bufferHandle,
            const VertexIndexListHandle indicies);
        AttributeView *newAttribView(
            const GLsizei attribOffset, const GLsizei attribLength,
            const GLsizei vertexSize);
    public:
        static GeometryBufferViewHandle create(
            const GenericGeometryBufferHandle bufferHandle,
            const VertexFormatHandle desiredFormat,
            const VertexIndexListHandle indicies);
    private:
        GenericGeometryBufferHandle _buffer;
        VertexFormatHandle _bufferFormat;
        VertexIndexListHandle _indicies;
    private:
        AttributeView
            *_position,
            *_colour,
            *_texCoord[BUFFER_TEX_COORD_COUNT],
            *_normal,
            *_vertexAttrib[BUFFER_VERTEX_ATTRIB_COUNT];
    public:
        GenericGeometryBufferHandle getHandle() { return _buffer; }
        GLsizei getLength() const { return _indicies->size(); }
    public:
        AttributeView *getPositionView() { return _position; };
        AttributeView *getColourView() { return _colour; };
        AttributeView *getTexCoordView(const unsigned int texCoordIndex);
        AttributeView *getNormalView() { return _normal; };
        AttributeView *getVertexAttribView(const unsigned int attribIndex);
};

}
}

#endif
