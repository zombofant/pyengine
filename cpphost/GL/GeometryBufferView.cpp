#include "GeometryBufferView.hpp"

namespace PyUni {
namespace GL {

/* PyUni::GL::GeometryBufferView */

GeometryBufferView::GeometryBufferView(
        const GenericGeometryBufferHandle buffer,
        const VertexIndexListHandle indicies):
    _buffer(buffer),
    _bufferFormat(buffer->getFormat()),
    _indicies(indicies),
    _position(newAttribView(_bufferFormat->posOffset, _bufferFormat->nPosition, _bufferFormat->vertexSize)),
    _colour(newAttribView(_bufferFormat->colourOffset, _bufferFormat->nColour, _bufferFormat->vertexSize)),
    _texCoord({
        newAttribView(_bufferFormat->texCoord0Offset, _bufferFormat->nTexCoord0, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->texCoord1Offset, _bufferFormat->nTexCoord1, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->texCoord2Offset, _bufferFormat->nTexCoord2, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->texCoord3Offset, _bufferFormat->nTexCoord3, _bufferFormat->vertexSize)
    }),
    _normal(newAttribView(_bufferFormat->normalOffset, (_bufferFormat->normal?3:0), _bufferFormat->vertexSize)),
    _vertexAttrib({
        newAttribView(_bufferFormat->vertexAttrib0Offset, _bufferFormat->nVertexAttrib0, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->vertexAttrib1Offset, _bufferFormat->nVertexAttrib1, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->vertexAttrib2Offset, _bufferFormat->nVertexAttrib2, _bufferFormat->vertexSize),
        newAttribView(_bufferFormat->vertexAttrib3Offset, _bufferFormat->nVertexAttrib3, _bufferFormat->vertexSize)
    })
{
    
}

GeometryBufferView::AttributeView *GeometryBufferView::newAttribView(
    const GLsizei attribOffset,
    const GLsizei attribLength,
    const GLsizei vertexSize)
{
    if (attribLength == 0)
        return 0;
    return new AttributeView(this, attribOffset, attribLength, vertexSize);
}

GeometryBufferViewHandle GeometryBufferView::create(
    const GenericGeometryBufferHandle buffer,
    const VertexFormatHandle desiredFormat,
    const VertexIndexListHandle indicies)
{
    if (!desiredFormat->isCompatible(*(buffer->getFormat().get()))) {
        return GeometryBufferViewHandle();
    }
    GeometryBufferView *view = new GeometryBufferView(buffer, indicies);
    return GeometryBufferViewHandle(view);
}

GeometryBufferView::AttributeView *GeometryBufferView::getTexCoordView(const unsigned int texCoordIndex)
{
    if (texCoordIndex >= BUFFER_TEX_COORD_COUNT)
    {
        return 0;
    }
    return _texCoord[texCoordIndex];
}

GeometryBufferView::AttributeView *GeometryBufferView::getVertexAttribView(const unsigned int vertexAttribIndex)
{
    if (vertexAttribIndex >= BUFFER_VERTEX_ATTRIB_COUNT)
    {
        return 0;
    }
    return _vertexAttrib[vertexAttribIndex];
}

/* PyUni::GL::GeometryBufferView::AttributeView */

GeometryBufferView::AttributeView::AttributeView(
        GeometryBufferView *view,
        const GLsizei attribOffset,
        const GLsizei attribLength,
        const GLsizei vertexSize):
    _view(view),
    _attribOffset(attribOffset),
    _attribLength(attribLength),
    _vertexSize(vertexSize),
    _slice(new AttributeSlice(this)),
    _vertexCount(view->getLength()),
    _vertexLength(view->getHandle()->getFormat()->vertexLength),
    _data(view->getHandle()->getData())
{

}

void GeometryBufferView::AttributeView::get(GLVertexFloat *data)
{
    slice(0, _vertexCount)->get(data);
}

GLsizei GeometryBufferView::AttributeView::getLength()
{
    return slice(0, _vertexCount)->getLength();
}

GLsizei GeometryBufferView::AttributeView::getSize()
{
    return slice(0, _vertexCount)->getSize();
}

GeometryBufferView::AttributeSlice *GeometryBufferView::AttributeView::slice(
    const GLsizei start, const GLsizei stop, const GLsizei step)
{
    GeometryBufferView::AttributeSlice *slice = _slice;
    slice->setUp(start, stop, step);
    return slice;
}

void GeometryBufferView::AttributeView::set(const GLVertexFloat *data)
{
    slice(0, _vertexCount)->set(data);
}

/* PyUni::GL::GeometryBufferView::AttributeSlice */

GeometryBufferView::AttributeSlice::AttributeSlice(AttributeView *view):
    _view(view),
    _start(0),
    _stop(0),
    _step(1)
{

}

void GeometryBufferView::AttributeSlice::setUp(const GLsizei start,
    const GLsizei stop, const GLsizei step)
{
    assert(start >= 0);
    assert(start <= stop);
    assert(step >= 1);
    assert(stop <= _view->_vertexCount);
    _start = start;
    _stop = stop;
    _step = step;
}

void GeometryBufferView::AttributeSlice::get(GLVertexFloat *data)
{
    GLsizei vertexLength = _view->_vertexLength;
    
    GLsizei step = _step * vertexLength;
    GLsizei len = getLength();
    GLsizei dstStep = _view->_attribLength;
    GLsizei attribSize = dstStep * sizeof(GLVertexFloat);

    GLVertexFloat *src = _view->_view->getHandle()->getData();
    src += _start * vertexLength + _view->_attribOffset;
    
    for (GLsizei i = 0; i < len; i++)
    {
        memcpy(data, src, attribSize);
        src += step;
        data += dstStep;
    }
}

}
}
