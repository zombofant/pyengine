# encoding=utf-8
# File name: Manager.py
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

from Base import ResourceLoader
from Engine.VFS.FileSystem import FileSystem

# Note: Available loaders are imported at the end of this file

class ResourceManager(object):
    """
    The resource manager provides access to resource loaders and caches
    already loaded resources for quick access. Every resource should be
    access trough the singleton instance of this class.

    This class should only be instantiated once. Use the .instance()
    method to get the instance of the ResourceManager.
    """
    __singleton = None

    # this is a simple singleton approach until we have a better one
    def __new__(cls, *args, **kwargs):
        if cls != type(cls.__singleton):
            cls.__singleton = super(type(ResourceManager), cls).__new__(cls, *args, **kwargs)
        return cls.__singleton

    def __init__(self, fileSystem=None, **kwargs):
        if not hasattr(self, '_initialized'):
            super(ResourceManager, self).__init__(**kwargs)
            self._resourceLoaders = {}
            self._resourceCache = {}
            self._fileSystem = None
            self._initialized = True

        # This bluntly and planned ignores the singletonness of
        # ResourceManager. This allows for convinient setting of
        # _fileSystem once.
        if self._fileSystem is None and fileSystem is not None:
            if not isinstance(fileSystem, FileSystem):
                raise TypeError("FileSystem expected, got {0} {1}".format(type(fileSystem), fileSystem))
            self._fileSystem = fileSystem

    def _open(self, uri, mode):
        if self._fileSystem is None:
            raise ValueError("Assign a file system first by calling ResourceManager(fileSystem=instance) first")
        return self._fileSystem.open(uri, mode)

    def _findResourceLoader(self, resourceType, requiredClass):
        """
        Tries to find an apropriate loader for the given resourceType. If a
        loader can be found, it is asked for its supported target classes.
        If it supports the given requiredClass, the loader will be returned.

        If no loader can be found for the requested resource type or the
        loader does not support the requested target class, an exception
        will be thrown.
        """
        if resourceType in self._resourceLoaders:
            loader = self._resourceLoaders[resourceType]
            if requiredClass is None:
                requiredClass = loader.defaultTargetClass()
            if requiredClass in loader.supportedTargetClasses():
                return loader
            else:
                raise Exception(
                    "Loader for type '{0}' does not support target class <{1}>"
                    .format(resourceType, requiredClass))
        else:
            raise Exception("No loader for type '{0}'".format(resourceType))

    def _resourceCacheStore(self, uri, instance):
        cacheId = "{0}_{1}".format(uri, type(instance))
        if cacheId in self._resourceCache: # maybe this is more worth a exception?
            del self._resourceCache[cacheId]
        self._resourceCache[cacheId] = instance

    def _resourceCacheRead(self, uri, requiredClass):
        cacheId = "{0}_{1}".format(uri, requiredClass)
        if cacheId in self._resourceCache:
            instance = self._resourceCache[cacheId]
            if not isinstance(instance, requiredClass):
                raise TypeError(
                    "Cached resource for {0} is of wrong class {1} ({2} requested)"
                    .format(uri, type(instance), requiredClass))
            return self._resourceCache[cacheId]
        return None

    def _resourceTypeFromURI(self, uri):
        pId = str(uri).rfind('.')
        if pId > -1:
            return uri[pId+1:].lower()
        raise TypeError('Cannot get resource type from uri: {0}'.format(uri))

    def require(self, uri, requiredClass=None, resourceType=None, **loaderArgs):
        """
        Load the given resource (or return cached instance).
        A resource can only be loaded if a loader is registered for the
        requested resourceType and it supports loading requiredClass

        If no type is given, the type is the requested file's extension.
        If no requiredClass is given, the loaders default is used (which
        should be the right choice for most cases)
        """
        instance = self._resourceCacheRead(uri, requiredClass)
        if instance is None:
            if resourceType is None:
                resourceType = self._resourceTypeFromURI(uri)
            loader = self._findResourceLoader(resourceType, requiredClass)
            resFile = self._open(uri, "r")
            instance = loader.load(resFile, requiredClass, **loaderArgs)
            # FIXME: close file handle
            if requiredClass is None:
                requiredClass = loader.defaultTargetClass()
            assert isinstance(instance, requiredClass) # sanity check
            self._resourceCacheStore(uri, instance)
        return instance

    def registerResourceLoader(self, loader):
        """
        Register a resource loader.
        The loader has to be a subclass of ResourceLoader.
        """
        assert isinstance(loader, ResourceLoader)
        for resourceType in loader.resourceTypes():
            if resourceType in self._resourceLoaders:
                raise Exception("Already registered a loader for type '{0}'".format(resourceType))
            self._resourceLoaders[resourceType] = loader

# import all available loaders here
from Text import TextLoader

