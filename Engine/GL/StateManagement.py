# File name: StateManagement.py
# This file is part of: pyuni
#
# LICENSE
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and limitations
# under the License.
#
# Alternatively, the contents of this file may be used under the terms
# of the GNU General Public license (the  "GPL License"), in which case
# the provisions of GPL License are applicable instead of those above.
#
# FEEDBACK & QUESTIONS
#
# For feedback and questions about pyuni please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

from Base import BindableObject
from pyglet.graphics import Group, OrderedGroup

class StateContext(object):
    def __init__(self, stateObj, **kwargs):
        super(StateContext, self).__init__(**kwargs)
        self._bindCall = self._stateObj.bind
        if isinstance(stateObj, BindableObject):
            self._unbindCall = type(self._stateObj).unbind
        elif isinstance(stateObj, StateContext):
            # FIXME: order the calls in a list instead of calling
            # recursively
            self._unbindCall = self._stateObj.unbind
        else:
            raise TypeError("StateContext requires a BindableObject or a StateContext. Got {0} {1}".format(type(stateObj), stateObj))
    
    def setContext(self):
        pass

    def bind(self):
        self.setContext()
        self._bindCall()

    def unbind(self):
        self.setContext()
        self._unbindCall()

class ActiveTexture(StateContext):
    """
    Makes sure that the active texture unit is set to the value passed
    in *textureUnit* before *stateObj* is bound/unbound.
    """
    
    def __init__(self, stateObj, textureUnit, **kwargs):
        super(ActiveTexture, self).__init__(stateObj, **kwargs)
        self._textureUnit = textureUnit

    def setContext(self):
        glActiveTexture(self._textureUnit)

class StateObjectGroup(Group):
    """
    Combines an arbitary amount of Groups, BindableObjects or
    StateContexts.

    The arguments, which must be instances of Group (pyglet),
    BindableObject (Engine.GL.Base) or StateContext
    (Engine.GL.StateManagement) will be taken in order. Their respective
    state setting and unsetting calls are merged together to be executed
    more efficiently.

    EXAMPLE:
        Having two GL.Texture2D instances ``texture1``, ``texture2``
        and some custom pyglet Group which implements lighting called
        ``lighting``, one could build a StateObjectGroup which
        enables and disables all of these togehter like this:

        StateObjectGroup(lighting,
            ActiveTexture(texture1, GL_TEXTURE0),
            ActiveTexture(texture2, GL_TEXTURE1))
    """
    
    def __init__(self, *args, **kwargs):
        parent = None
        # 2to3: this can be made better in python3 using keyword-only
        # arguments
        if "parent" in kwargs:
            parent = kwargs["parent"]
            del kwargs["parent"]
        super(Group, self).__init__()
        setCalls = []
        unsetCalls = []
        for stateObj in args:
            if isinstance(stateObj, Group):
                setCalls.append(stateObj.set_state)
                unsetCalls.append(stateObj.unset_state)
            elif isinstance(stateObj, BindableObject):
                setCalls.append(stateObj.bind)
                unsetCalls.append(type(stateObj).unbind)
            elif isinstance(stateObj, StateContext):
                setCalls.append(stateObj.bind)
                unsetCalls.append(stateObj.unbind)
        unsetCalls.reverse()
        self._setCalls = tuple(setCalls)
        self._unsetCalls = tuple(unsetCalls)

    def set_state(self):
        for call in self._setCalls:
            call()

    def unset_state(self):
        for call in self._unsetCalls:
            call()

    def __hash__(self):
        return hash((self._setCalls, self._unsetCalls))

class OrderedStateObjectGroup(StateObjectGroup):
    def __init__(self, *args, **kwargs):
        order = 0
        # 2to3: keyword-only argument
        if "order" in kwargs:
            order = kwargs["order"]
            del kwargs["order"]
        super(OrderedStateObjectGroup, self).__init__(*args, **kwargs)
        self.order = order

    # XXX: Is this a correct implementation? Mainly copied from
    # pyglet/graphics/__init__.py, however it seems wrong to me.
    def __cmp__(self, other):
        if isinstance(other, (OrderedStateObjectGroup, OrderedGroup)):
            return cmp(self.order, other.order)
        return -1

    def __eq__(self, other):
        return (
            (self.__class__ == other.__class__ or OrderedGroup == other.__class__) and
            (self.order == other.order) and
            (self.parent == other.parent))

    def __hash__(self):
        return hash((self.order, self.parent, self._setCalls, self._unsetCalls))
