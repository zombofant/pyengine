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

    def __init__(self, **kwargs):
        # do not allow direct instanciation
        assert self.__class__ != ResourceLoader
        super(ResourceLoader, self).__init__(**kwargs)

    def load(self, fileLike, targetClass=None, **loaderArgs):
        """
        The actual loader that returns the loaded instance.
        This has to be overwritten by all subclasses.
        """
        raise Exception('You forgot to overwrite the load() method!')
        return None

    def supportedTargetClasses(self):
        """
        Return the list of supported target classes.
        This has to be overwritten by all subclasses.
        """
        raise Exception('You forgot to overwrite supportedTargetClasses!')
        return []

    def defaultTargetClass(self):
        """
        Return the default target class this loader creates when loading.
        Defaults to the first class in supportedTargetClasses().
        This method may be overwritten by subclasses if necessary.
        """
        return self.supportedTargetClasses()[0]

    def resourceTypes(self):
        """
        Return the resource type this loader is able to load.
        This method has to be overwritten by all subclasses.
        """
        raise Exception('You forgot to overwrite resourceType()!')
        return []

# every resource loader should have a line like the one below at the end of
# its file in order to register an instance of itself with the resource manager
#from Manager import ResourceManager
#ResourceManager().registerResourceLoader(MyResourceLoader())

