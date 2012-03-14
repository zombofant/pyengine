/**********************************************************************
File name: GeometryBufferView.cpp
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
#include "GeometryBufferView.hpp"
#include "GeometryObject.hpp"

namespace PyUni {
namespace GL {

/* PyUni::GL::GeometryBufferView */

GeometryBufferView::GeometryBufferView(
        const GeometryBufferHandle buffer,
        const VertexIndexListHandle indicies):
    _buffer(buffer),
    _bufferFormat(buffer->getFormat()),
    _indicies(indicies),
    _map(new VertexIndexListMap(indicies)),
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
    return new AttributeView(this, attribOffset / sizeof(GLVertexFloat), attribLength, vertexSize);
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

GLsizei GeometryBufferView::AttributeView::getAttributeLength()
{
    return _attribLength;
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
    const GLsizei start, const GLsizei stop, const GLsizei step,
    const GLsizei attribOffset, const GLsizei attribLength)
{
    GeometryBufferView::AttributeSlice *slice = _slice;
    slice->setUp(start, stop, step, _attribOffset + attribOffset, (attribLength>=1?attribLength:_attribLength));
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
    _step(1),
    _attribOffset(_view->_attribOffset),
    _attribLength(_view->_attribLength)
{

}

void GeometryBufferView::AttributeSlice::setUp(const GLsizei start,
    const GLsizei stop, const GLsizei step, const GLsizei attribOffset,
    const GLsizei attribLength)
{
    assert(start >= 0);
    assert(start <= stop);
    assert(step >= 1);
    assert(stop <= _view->_vertexCount);
    assert(attribOffset >= _view->_attribOffset);
    assert(attribLength <= (_view->_attribLength - (attribOffset-_view->_attribOffset)));
    _start = start;
    _stop = stop;
    _step = step;
    _attribOffset = attribOffset;
    _attribLength = attribLength;
}

void GeometryBufferView::AttributeSlice::get(GLVertexFloat *data)
{
    GLsizei vertexLength = _view->_vertexLength;
    GLsizei attribOffset = _attribOffset;
    GLsizei dstStep = _attribLength;

    const GLsizei copySize = _attribLength * sizeof(GLVertexFloat);

    GLVertexFloat *src = _view->_view->_buffer->getData();
    BufferMap *map = _view->_view->_map;

    GLVertexFloat *dest = data;
    for (GLsizei i = _start; i < _stop; i += _step)
    {
        const GLsizei actualIndex = map->map(i);
        const GLsizei floatIndex = actualIndex * vertexLength + attribOffset;
        memcpy(dest, &src[floatIndex], copySize);
        dest += dstStep;
    }
}

GLsizei GeometryBufferView::AttributeSlice::getAttributeLength()
{
    return _attribLength;
}

GLsizei GeometryBufferView::AttributeSlice::getLength()
{
    return ceil(((double)(_stop - _start)) / ((double)_step));
}

GLsizei GeometryBufferView::AttributeSlice::getSize()
{
    return sizeof(GLVertexFloat) * getLength() * getAttributeLength();
}

void GeometryBufferView::AttributeSlice::set(const GLVertexFloat *data)
{
    GLsizei vertexLength = _view->_vertexLength;
    GLsizei attribOffset = _attribOffset;
    GLsizei srcStep = _attribLength;

    const GLsizei copySize = _attribLength * sizeof(GLVertexFloat);

    GLVertexFloat *dest = _view->_view->_buffer->getData();
    BufferMap *map = _view->_view->_map;

    const GLVertexFloat *src = data;

    GLsizei minIndex = -1, maxIndex = -1;

    for (GLsizei i = _start; i < _stop; i += _step)
    {
        const GLsizei actualIndex = map->map(i);
        minIndex = ((actualIndex<minIndex) || (minIndex == -1)?actualIndex:minIndex);
        minIndex = ((actualIndex>maxIndex) || (maxIndex == -1)?actualIndex:maxIndex);
        
        const GLsizei floatIndex = actualIndex * vertexLength + attribOffset;
        memcpy(&dest[floatIndex], src, copySize);
        src += srcStep;
    }

    _view->_view->_buffer->invalidateRange(minIndex, maxIndex);
}


}
}
