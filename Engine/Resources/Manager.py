# encoding=utf-8
# File name: Manager.py
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

    def __init__(self, filesystem=None, **kwargs):
        if not hasattr(self, '_initialized'):
            super(ResourceManager, self).__init__(**kwargs)
            self._resource_loaders = {}
            self._resource_cache = {}
            self._filesystem = None
            self._initialized = True

        # This bluntly and planned ignores the singletonness of
        # ResourceManager. This allows for convinient setting of
        # _filesystem once.
        if self._filesystem is None and filesystem is not None:
            if not isinstance(filesystem, FileSystem):
                raise TypeError("FileSystem expected, got {0} {1}".format(type(filesystem), filesystem))
            self._filesystem = filesystem

    def _open(self, uri, mode):
        if self._filesystem is None:
            raise ValueError("Assign a file system first by calling ResourceManager(filesystem=instance) first")
        return self._filesystem.open(uri, mode)

    def _find_resource_loader(self, resourcetype, required_class):
        """
        Tries to find an apropriate loader for the given resourcetype. If a
        loader can be found, it is asked for its supported target classes.
        If it supports the given required_class, the loader will be returned.

        If no loader can be found for the requested resource type or the
        loader does not support the requested target class, an exception
        will be thrown.
        """
        if resourcetype in self._resource_loaders:
            loader = self._resource_loaders[resourcetype]
            if required_class is None:
                required_class = loader.DefaultTargetClass
            if required_class in loader.SupportedTargetClasses:
                return loader
            else:
                raise NotImplementedError(
                    "Loader for type '{0}' does not support target class <{1}>"
                    .format(resourcetype, required_class))
        else:
            raise NotImplementedError("No loader for type '{0}'".format(resourcetype))

    def _resource_cache_store(self, cachetoken, instance):
        if cachetoken in self._resource_cache: # maybe this is more worth a exception?
            del self._resource_cache[cachetoken]
        self._resource_cache[cachetoken] = instance

    def _resource_cache_read(self, loader_class, uri, required_class, loaderargs):
        cachetoken = loader_class.get_cache_token(uri, required_class, **loaderargs)
        if cachetoken is not None and cachetoken in self._resource_cache:
            instance = self._resource_cache[cachetoken]
            if not isinstance(instance, required_class):
                raise TypeError(
                    "Cached resource for {0} is of wrong class {1} ({2} requested)"
                    .format(uri, type(instance), required_class))
            return cachetoken, self._resource_cache[cachetoken]
        return cachetoken, None

    def _resource_type_from_uri(self, uri):
        ext = os.path.splitext(uri)[1]
        if len(ext) < 2:
            raise TypeError('Cannot get resource type from uri: {0}'.format(uri))
        return ext[1:]

    def require(self, uri, required_class=None, resourcetype=None, **loaderargs):
        """
        Load the given resource (or return cached instance).
        A resource can only be loaded if a loader is registered for the
        requested resourcetype and it supports loading required_class

        If no type is given, the type is the requested file's extension.
        If no required_class is given, the loaders default is used (which
        should be the right choice for most cases)
        """
        if len(uri) < 1:
            raise ValueError('uri must not be empty!')
        if resourcetype is None:
            resourcetype = self._resource_type_from_uri(uri)
        
        loader = self._find_resource_loader(resourcetype, required_class)
        if required_class is None:
            required_class = loader.DefaultTargetClass
        if uri[0] != '/':
            if loader.RelativePathPrefix is None:
                raise ValueError("An absolute URI is required to load a {0} resource.".format(loader))
            else:
                uri = Utils.join(loader.RelativePathPrefix, uri)
        
        cachetoken, instance = self._resource_cache_read(loader, uri, required_class, loaderargs)
        if instance is None:
            resfile = self._open(uri, "r")
            instance = loader.load(resfile, required_class, **loaderargs)
            resfile.close()
            assert isinstance(instance, required_class) # sanity check
            if cachetoken is not None:
                self._resource_cache_store(cachetoken, instance)
        return instance

    def register_resource_loader(self, loader_class, **kwargs):
        """
        Register a resource loader.
        The loader has to be a subclass of ResourceLoader.
        """
        try:
            loader = loader_class(**kwargs)
        except NotImplementedError as err:
            log.log(Severity.Warning, "Loader {0!s} is not available. Reason: {1!s}".format(loader_class, err))
            return
        for resourcetype in loader.ResourceTypes:
            if resourcetype in self._resource_loaders:
                raise Exception("Already registered a loader for type '{0}'".format(resourcetype))
            self._resource_loaders[resourcetype] = loader

    def clear_cache(self):
        self._resource_cache = {}
