:mod:`GL` – Fast OpenGL data management
=======================================

.. currentmodule:: Engine.CEngine.GL

Geometry buffer management
--------------------------

.. function:: VertexFormat(fmtSpecifier)

    Return a C++ compatible data structure to setup and configure
    :class:`GeometryBuffer` instances.

    The format specifier must be a string which defines the vertex
    format in a way similar to pyglet. The string consists of multiple
    `Attribute Specifiers`, separated by semicolons.

    Each `Attribute Specifier` consists of a `Attribute Type` and a
    count giving the number of values to associate with that attribute.
    The `Attribute Type` must be one of the following:

    +---------------------------+-----------+-------+
    | OpenGL attribute          | Specifier | Notes |
    +===========================+===========+=======+
    | Vertex                    | ``v``     |       |
    +---------------------------+-----------+-------+
    | Colour                    | ``c``     |       |
    +---------------------------+-----------+-------+
    | Texture coordinate *i*    | ``ti``    | \(1)  |
    +---------------------------+-----------+-------+
    | Normal                    | ``n``     | \(2)  |
    +---------------------------+-----------+-------+
    | Generic attribute *i*     | ``gi``    | \(1)  |
    +---------------------------+-----------+-------+

    Notes:

    (1)
        ``i`` is to be replaced by the index of the texture
        unit/generic attribute, starting with 0.

    (2)
        Due to a restriction in OpenGL, normals must always have a
        value count of 3.
    
    So a valid format would look like::
    
        VertexFormat("v:2;t0:2;c:4")

    This would give you a vertex format with two vertex coordinates
    (XY), two texture coordinates (UV) and four colour values (RGBA).

    The order of attributes in the geometry buffer is not affected by
    the order of attributes in the vertex format string.


.. class:: GeometryBuffer(vertexFormat, usage)

    Manage a memory area for geometry storage on the graphics board.

    *vertexFormat* must be a valid vertex format object as created using
    :func:`VertexFormat`. *usage* is equivalent to OpenGL
    :func:`glBufferData` argument *usage* and accepts the same values.

    .. method:: allocateVertices(count)
    
        Allocate *count* vertex structures. Return a
        :class:`VertexIndexList` containing the (not neccessarily
        consecutive or monotonous) indicies for the vertices.
        
        This may invoke garbage collection in the buffer.
        
    .. method:: bind()
    
        Bind the buffer for use with OpenGL.

        This may invoke garbage collection in the buffer and transfer
        data to the graphics card, if the buffer is dirty.
    
    .. method:: draw(vertexIndexList, mode)
    
        Draw the indicies from the given *vertexIndexList* using the
        OpenGL primitive mode *mode*.

    .. method:: gc()
    
        Execute garbage collection on the geometry buffer. This purges
        all allocations for which no handles are around anymore.

    .. method:: unbind()
    
        Unbind the buffer from OpenGL

.. class:: GeometryBufferView(geometryBuffer, vertexIndexList)

    Manages a view on the given *geometryBuffer*, in which the
    vertices from *vertexIndexList* appear to be consecutive. Using
    the attribute views, this allows for a convienient access on
    geometry data in buffers::

        # create a buffer
        buffer = GeometryBuffer(VertexFormat("v:2"), GL.GL_STATIC_DRAW)
        # allocate vertices
        vertices = buffer.allocateVertices(3)
        # get a view on the buffer for the vertices
        view = GeometryBufferView(buffer, vertices)
        # set up a triangle with vertices (0, 0), (0, 1), (1, 1)
        view.Vertex[0:3].set([
            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0
        ])

    .. property:: Vertex, Colour, Normal
    
        Access to the :class:`AttributeView` instances of this
        geometry buffer view for the respective attributes.
    
    .. method:: TexCoord(index)
    
        Access to the :class:`AttributeView` instance of this
        geometry buffer view for the *index*th texture coordinate.

    .. method:: Attrib(index)
    
        Access to the :class:`AttributeView` instance of this
        geometry buffer view for the *index*th generic attribute.
        

.. class:: AttributeView

    Attribute views represent the values of an geometry attribute in a
    :class:`GeometryBufferView`. Access to views is reasonably fast.
    Views can be sliced using the usual python slice operations and
    support len().

    .. property:: AttributeLength
    
        Number of float values associated with the attribute.
        
    .. property:: Size
    
        Size of the attribute view in bytes.
        
    .. method:: get()
    
        Return the contents of the attribute in the associated
        :class:`GeometryBufferView` as list of floats.

        Note: this may change to a numpy array in the future.
        
    .. method:: set(data)
    
        Set the whole attribute view to the floats given in *data*.
        *data* is expected to be a python list of floats and have the
        same length as the attribute view.
        

.. class:: AttributeSlice

    .. property:: Size
    
        Size of the attribute slice in bytes.
        
    .. method:: get()
    
        Return the attribute slice contents as python list of floats.

        Note: this may change to a numpy array in the future.
        
    .. method:: set(data)
    
        Set the attribute slice. This works the same as
        :meth:`AttributeView.set`, except that it only influences the
        given slice.


Index buffer management
-----------------------

.. class:: GenericIndexBuffer

    Base class for both :class:`StreamIndexBuffer` and
    :class:`StaticIndexBuffer`. You can not create instances of this
    class by yourself. It is documented here to document the
    common methods of the index buffer classes.

    .. method:: bind()
    
        Bind the buffer for usage with OpenGL.

        This may cause data to be transferred to the graphics card.
    
    .. method:: clear()
        
        Clear the contents of the buffer.
        
    .. method:: draw(mode)

        Draw the contents of the buffer using the OpenGL primitive
        mode *mode*.

        The buffer must be bound beforehand.
    
    .. method:: drawUnbound(mode)

        Draw the contents of the buffer using the OpenGL primitive mode
        *mode*, but without the need to bind the buffer beforehand. This
        is slower than :meth:`draw`, as the data needs to be transferred
        to the GPU first. For static index buffers, you usually want to
        use :meth:`draw`.

        However, this can be useful for quickly changing indicies, as
        the overhead of storing the data on the GPU would be larger.
    
    .. method:: dump()

        Debug output to stdout.
    
    .. method:: unbind()

        Unbind the buffer from OpenGL.

.. class:: StreamIndexBuffer(usage)

    An index buffer implementation which is optimized for quickly
    changing contents. It does not keep any internal state on the
    buffers contents except length and capacity, so adding and clearing
    is a quick operation.

    *usage* corresponds to the OpenGL buffer usage hint as given to
    :func:`glBufferData`.

    .. method:: add(vertices)
    
        Add a :class:`VertexIndexList` *vertices* to the stream buffer.

.. class:: StaticIndexBuffer(usage)

    This index buffer is better suited for contents which do not change
    a lot or where only parts change. It essentially implements a simple
    memory manager which allows for dynamic addition and removal of
    blocks. Only changed parts are (re-)transferred to the GPU.

    *usage* corresponds to the OpenGL buffer usage hint as given to
    :func:`glBufferData`.

    .. method:: add(vertices)

        Add a :class:`VertexIndexList` *vertices* to the index buffer.
        Return a :class:`IndexEntry` which can be used to remove
        the indicies from the buffer.
    
    .. method:: drawHandle(indexEntry, mode)

        Draw a single :class:`IndexEntry` using the OpenGL primitive
        mode *mode*. This does not require the buffer to be bound, it is
        drawn from the main memory.
        
    .. method:: gc()

        Invoke garbage collection on the index buffer. This will remove
        any unreferenced :class:`IndexEntry` instances to free up their
        space in the buffer allocation.
    
    .. method:: getIndex(index)

        Return the vertex index stored at *index* in the buffer.
        
    .. method:: remove(indexEntry, autoCompress)

        Remove the index entry (must be of instance :class:`IndexEntry`)
        from the buffer. If *autoCompress* is true, garbage collection
        is invoked and indicies are rearranged in the buffer for optimal
        performance.
    
    .. method:: resolveIndexEntry(indexEntry)

        Return the :class:`VertexIndexList` associated with the given
        *indexEntry*.
