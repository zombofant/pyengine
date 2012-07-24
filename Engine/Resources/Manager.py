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

import os
import sys

from Engine.CEngine.Log import Severity

from Base import ResourceLoader, log
from Engine.VFS.FileSystem import FileSystem
import Engine.VFS.Utils as Utils

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
                requiredClass = loader.DefaultTargetClass
            if requiredClass in loader.SupportedTargetClasses:
                return loader
            else:
                raise NotImplementedError(
                    "Loader for type '{0}' does not support target class <{1}>"
                    .format(resourceType, requiredClass))
        else:
            raise NotImplementedError("No loader for type '{0}'".format(resourceType))

    def _resourceCacheStore(self, cacheToken, instance):
        if cacheToken in self._resourceCache: # maybe this is more worth a exception?
            del self._resourceCache[cacheToken]
        self._resourceCache[cacheToken] = instance

    def _resourceCacheRead(self, loaderClass, uri, requiredClass, loaderArgs):
        cacheToken = loaderClass.getCacheToken(uri, requiredClass, **loaderArgs)
        if cacheToken is not None and cacheToken in self._resourceCache:
            instance = self._resourceCache[cacheToken]
            if not isinstance(instance, requiredClass):
                raise TypeError(
                    "Cached resource for {0} is of wrong class {1} ({2} requested)"
                    .format(uri, type(instance), requiredClass))
            return cacheToken, self._resourceCache[cacheToken]
        return cacheToken, None

    def _resourceTypeFromURI(self, uri):
        ext = os.path.splitext(uri)[1]
        if len(ext) < 2:
            raise TypeError('Cannot get resource type from uri: {0}'.format(uri))
        return ext[1:]

    def require(self, uri, requiredClass=None, resourceType=None, **loaderArgs):
        """
        Load the given resource (or return cached instance).
        A resource can only be loaded if a loader is registered for the
        requested resourceType and it supports loading requiredClass

        If no type is given, the type is the requested file's extension.
        If no requiredClass is given, the loaders default is used (which
        should be the right choice for most cases)
        """
        if len(uri) < 1:
            raise ValueError('uri must not be empty!')
        if resourceType is None:
            resourceType = self._resourceTypeFromURI(uri)
        
        loader = self._findResourceLoader(resourceType, requiredClass)
        if requiredClass is None:
            requiredClass = loader.DefaultTargetClass
        if uri[0] != '/':
            if loader.RelativePathPrefix is None:
                raise ValueError("An absolute URI is required to load a {0} resource.".format(loader))
            else:
                uri = Utils.join(loader.RelativePathPrefix, uri)
        
        cacheToken, instance = self._resourceCacheRead(loader, uri, requiredClass, loaderArgs)
        if instance is None:
            resFile = self._open(uri, "r")
            instance = loader.load(resFile, requiredClass, **loaderArgs)
            resFile.close()
            assert isinstance(instance, requiredClass) # sanity check
            if cacheToken is not None:
                self._resourceCacheStore(cacheToken, instance)
        return instance

    def registerResourceLoader(self, loaderClass, **kwargs):
        """
        Register a resource loader.
        The loader has to be a subclass of ResourceLoader.
        """
        try:
            loader = loaderClass(**kwargs)
        except NotImplementedError as err:
            log.log(Severity.Warning, "Loader {0!s} is not available. Reason: {1!s}".format(loaderClass, err))
            return
        for resourceType in loader.ResourceTypes:
            if resourceType in self._resourceLoaders:
                raise Exception("Already registered a loader for type '{0}'".format(resourceType))
            self._resourceLoaders[resourceType] = loader

    def clearCache(self):
        self._resourceCache = {}
