# File name: Base.py
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

class ResourceLoader(object):
    """
    The base class for all resource loader classes.
    See the TextLoader in TextLoader.py for a reference on how to implement
    a resource loader class using ResourceLoader.

    Every resource loader should register itself to the resource manager
    by calling the managers registerResourceLoader() method in order to
    be of any use.
    """

    def __init__(self, **kwargs):
        """
        Constructs a ResourceLoader class. ResourceLoader should never be
        instantiated directly. All subclasses have to set the following
        attributes to their apropriate values:
            _supportedTargetClasses
            _defaultTargetClass
            _resourceTypes
        See the property definitions below for more information on this
        attributes and how they should look like.
        """
        super(ResourceLoader, self).__init__(**kwargs)
        self._supportedTargetClasses = []
        self._defaultTargetClass = None
        self._resourceTypes = []

    def load(self, fileLike, targetClass=None, **loaderArgs):
        """
        The actual loader that returns the loaded instance.
        This has to be overwritten by all subclasses.
        """
        raise NotImplemented('You forgot to overwrite the load() method!')
        return None

    @property
    def supportedTargetClasses(self):
        """
        The list of supported target classes.
        This property to be set by all subclasses.
        """
        return self._supportedTargetClasses

    @property
    def defaultTargetClass(self):
        """
        The default target class this loader creates when loading.
        This property may be set by subclasses if necessary.
        """
        return self._defaultTargetClass

    @property
    def resourceTypes(self):
        """
        The resource type this loader is able to load.
        This property has to be set by all subclasses.
        """
        return self._resourceTypes

