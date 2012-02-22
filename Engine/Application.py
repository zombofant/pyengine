# File name: Application.py
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
from PySFML import sf
import time

"""
This class basically implements a game loop efficiently. We currently
only support one window; We do not need more now.

.. note::
    If you override methods after the Window.__init__ was executed, you
    also have to change the method reference in the *eventHandlers* 
    dictionary.

"""
class Window(object):
    EventHandlers = {
        sf.Event.Closed: "onClosed",
        sf.Event.Resized: "onResized",
        sf.Event.LostFocus: "onLostFocus",
        sf.Event.GainedFocus: "onGainedFocus",
        sf.Event.TextEntered: "onTextEntered",
        sf.Event.KeyPressed: "onKeyPressed",
        sf.Event.KeyReleased: "onKeyReleased",
        sf.Event.MouseWheelMoved: "onMouseWheelMoved",
        sf.Event.MouseButtonPressed: "onMouseButtonPressed",
        sf.Event.MouseButtonReleased: "onMouseButtonReleased",
        sf.Event.MouseMoved: "onMouseMoved",
        sf.Event.MouseEntered: "onMouseEntered",
        sf.Event.MouseLeft: "onMouseLeft",
        sf.Event.JoyButtonPressed: "onJoyButtonPressed",
        sf.Event.JoyButtonReleased: "onJoyButtonReleased",
        sf.Event.JoyMoved: "onJoyMoved"
    }
    
    def __init__(self, 
            initialGeometry=None,
            initialTitle=None,
            sfVideoMode=None,
            **kwargs):
        if (sfVideoMode is not None) and (initialGeometry is not None):
            raise ValueError("Only one of sfVideoMode and initialGeometry must be set.")
        w, h = (int(x) for x in initialGeometry or (800, 600))
        if w < 0 or h < 0:
            self._raiseDimensionsTooSmall(w, h)

        if(initialTitle is None):
            initialTitle = self.__class__.__name__
            
        self._sfVideoMode = sfVideoMode or sf.VideoMode(w, h)
        self._sfStyles = sf.Style.Resize | sf.Style.Close
        super(Window, self).__init__(**kwargs)
        self._initEventHandlers()
        self.window = sf.Window(self._sfVideoMode, initialTitle, self._sfStyles)
        self.window.UseVerticalSync(True)
        
        self._width, self._height = self.window.GetWidth(), self.window.GetHeight()
        self._title = initialTitle
        self._vsync = True
        self._terminated = True
        self._syncedFrameLength = 0.01
        self.clock = sf.Clock()
    
    def _initEventHandlers(self):
        self.eventHandlers = {}
        for evType, methName in self.EventHandlers.iteritems():
            try:
                method = getattr(self, methName)
                self.eventHandlers[evType] = method
            except:
                pass
    
    def _raiseDimensionsTooSmall(self, w, h):
        raise ValueError("Width and height must be positive (got w={0}, h={1})".format(w, h))
    
    def render(self):
        """
        This method is called at the end of any game loop iteration, 
        just before a skippedFrame call (or the next iteration, if no
        frame skipping takes place).
        """
    
    def updateSynced(self):
        """
        This method gets called every *SyncedFrameLength* seconds. It
        may be called several times in sequence if the previous frame 
        took too long, but each call still represents the guaranteed 
        amount of time specified.
        """
    
    def updateUnsynced(self, timeDelta):
        """
        This method is called once in every game loop iteration. So an
        arbitary amount of time may have passed, which is given by the
        *timeDelta* argument.
        """
    
    def run(self):
        """
        The run method is fully local, except for the *_terminated*
        attribute. Everything else is pulled into the run scope, so 
        you cannot change any attributes of a *Window* instance so that 
        this method takes note if it.
        
        An exception are those attributes which are just wrappers for
        attributes of the lower level, like Height, Width and VSync.
        """
        
        self._terminated = False
        clock = self.clock
        window = self.window
        event = sf.Event()
        syncedFrameLength = self._syncedFrameLength
        
        timeDelta = 0.
        thisTimeDelta = 0.
        while not self._terminated:
            while window.GetEvent(event):
                if event.Type in self.eventHandlers:
                    self.eventHandlers[event.Type](event)
            
            thisTimeDelta = clock.GetElapsedTime()
            clock.Reset()
            timeDelta += thisTimeDelta
            skipFrame = True
            while timeDelta >= syncedFrameLength:
                skipFrame = False
                timeDelta -= syncedFrameLength
                self.updateSynced()
        
            self.updateUnsynced(timeDelta)
            self.render()
            if skipFrame:
                skippedFrame = True
    
    def skippedFrame(self):
        """
        This only gets called if a frame went on too fast, so that the
        threshold given by *SyncedFrameLength* was not passed. By 
        default, this implements a sleep call for a time of
        ``SyncedFrameLength/2.``
        """
        time.sleep(self._syncedFrameLength / 2.)
    
    def terminate(self):
        self._terminated = True
        
    def onClosed(self, event):
        """
        Calls ``self.terminate()``.
        """
        self.terminate()
        
    def onResized(self, event):
        """
        Adjusts the properties. Make sure you call this per super() if
        you override this method.
        """
        self._width, self._height = event.Size.Width, event.Size.Height
        
    @property
    def Width(self):
        return self._width
        
    @property
    def Height(self):
        return self._height
        
    @property
    def Dimensions(self):
        return (self._width, self._height)
        
    @Dimensions.setter
    def Dimensions(self, value):
        w, h = (int(x) for x in value)
        if w < 0 or h < 0:
            self._raiseDimensionsTooSmall(w, h)
        self._width, self._height = w, h
        self.window.SetSize(w, h)
    
    @property
    def Title(self):
        return self._title
    
    @Title.setter
    def Title(self, value):
        """
        Recreates the window with the new title using SFMLs create().
        Unfortunately, PySFML does not seem to provide a simpler way to
        do this at the moment.
        """
        settings = self.window.GetSettings()
        self.window.Create(self._sfVideoMode, value, self._sfStyles, settings)
        self._title = value
        
    @property
    def VSync(self):
        return self._vsync
    
    @VSync.setter
    def VSync(self, value):
        value = bool(value)
        if value == self._vsync:
            return
        self.window.UseVerticalSync(value)
        self._vsync = value
        
    @property
    def SyncedFrameLength(self):
        return self._syncedFrameLength
    
    @SyncedFrameLength.setter
    def SyncedFrameLength(self, value):
        assert not self._terminated
        self._syncedFrameLength = float(value)

