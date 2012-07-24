:mod:`Border` – Border handling
===============================
.. currentmodule:: Engine.UI.CSS.Border

This module implements a border component used in CSS layout rendering.
The :class:`Border` class manages a 3×3 box border, using four
:class:`BorderEdge` instances and four border corner radii.

.. autoclass:: BorderComponent
    :members:

.. autoclass:: Border
    :members: geometryForRect, inCairo, setWidth, setFill, setRadius,
        getBox

    .. attribute::  Top
                    Left
                    Right
                    Bottom
        
        Access to the :class:`BorderEdge` instances used for the
        respective edges by this :class:`Border`.

        When assigning another :class:`BorderComponent` instance to any
        of these properties, the instance will not be referenced.
        Instead, its values are transferred using the
        :meth:`BorderEdge.assign` method.

    .. attribute::  TopLeftRadius
                    TopRightRadius
                    BottomLeftRadius
                    BottomRightRadius

        Access the rounded edge radii of the border for each corner
        respectively.

.. autoclass:: BorderEdge
    :members:
    :inherited-members: assign
