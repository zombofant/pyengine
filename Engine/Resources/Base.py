# File name: Base.py
# This file is part of: pyengine
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
# For feedback and questions about pyengine please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

from Engine.CEngine.Log import server, Severity
log = server.getChannel("resources")

class ResourceLoader(object):
    """
    The base class for all resource loader classes.
    See the TextLoader in TextLoader.py for a reference on how to implement
    a resource loader class using ResourceLoader.

    Every resource loader should register itself to the resource manager
    by calling the managers register_resource_loader() method in order to
    be of any use.
    """

    @classmethod
    def _loader_not_available(cls, error=None):
        raise NotImplementedError(error or ("{0!s} is not implemented on this platform.".format(cls)))

    def __init__(self,
            supported_target_classes,
            resource_types,
            relative_path_prefix=None,
            default_target_class=None,
            **kwargs):
        """
        Creates a ResourceLoader class.

        ResourceLoaders should never be instanciated directly. The
        ResourceManager takes care about that already.

        *supported_target_classes* must be an iterable (``frozenset`` is
        preferred) which yields the python classes the loader can
        construct.
        
        *resource_types* must be a iterable (``frozenset`` is preferred)
        which yields the file extensions (without leading ``"."``) which
        are supported by the loader.

        *relative_path_prefix* can be a string which is prepended to the
        path if a relative path is given upon loading. Defaults to None,
        which marks a need for absolute paths.

        *default_target_class* can be a class which must also be in
        *supported_target_classes* which is passed to the load method if
        the client does not specify a class on its own. If it is None,
        the first class from the *supported_target_classes* iterable is
        taken (for this, *supported_target_classes* must be subscriptable
        obviously).
        """
        if len(supported_target_classes) == 0 and default_target_class is None:
            self._loader_not_available()
        super(ResourceLoader, self).__init__(**kwargs)
        self._supported_target_classes = frozenset(supported_target_classes)
        self._default_target_class = default_target_class or supported_target_classes[0]
        self._resource_types = frozenset(resource_types)
        self._relative_path_prefix = unicode(relative_path_prefix)

    def load(self, filelike, targetclass=None, **loaderargs):
        """
        The actual loader that returns the loaded instance.
        This has to be overwritten by all subclasses.
        """
        raise NotImplemented()

    def get_cache_token(self, vfspath, targetclass=None, **loaderargs):
        """
        Must return a valid dict key which depends on any cache relevant
        loader arguments.

        By default, this returns ``(vfspath,)``.
        """
        return (vfspath,)

    @property
    def SupportedTargetClasses(self):
        """
        The list of supported target classes.
        This property to be set by all subclasses.
        """
        return self._supported_target_classes

    @property
    def DefaultTargetClass(self):
        """
        The default target class this loader creates when loading.
        This property may be set by subclasses if necessary.
        """
        return self._default_target_class

    @property
    def ResourceTypes(self):
        """
        The resource type this loader is able to load.
        This property has to be set by all subclasses.
        """
        return self._resource_types

    @property
    def RelativePathPrefix(self):
        """
        The prefix to be prepended in relative paths.
        If there is a request for 'somefile.ext', the loaders
        relative_path_prefix will be prepended before opening the file.
        """
        return self._relative_path_prefix

