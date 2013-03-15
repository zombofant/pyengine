# encoding=utf-8
# File name: FSWidget.py
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

__all__ = ["FileList", "FileListItem", "FilePicker"]

import os
import stat
import functools

import CSS.Minilanguage

from ListWidget import List, TextListItem
from WindowWidget import Window
from BoxWidget import HBox
from ButtonWidget import Button

@functools.total_ordering
class FileListItem(TextListItem):
    def __init__(self, parent, path, **kwargs):
        self._path = path
        self._is_directory = stat.S_ISDIR(os.stat(path).st_mode)
        super(FileListItem, self).__init__(parent,
                                           os.path.basename(path), **kwargs)

    def __lt__(self, oth):
        if oth._is_directory and not self._is_directory:
            return False
        elif not oth._is_directory and self._is_directory:
            return True
        else:
            return self._path < oth._path

    def __eq__(self, oth):
        if not isinstance(oth, FileListItem):
            return False
        return oth._path == self._path

    def on_mouse_click(self, x, y, button, modifiers, nth):
        if nth == 2:
            if self._is_directory:
                self.Parent.Directory = self._path
                return True
            else:
                self.Parent._file_dbl_click(self)
                return True
        return super(FileListItem, self).on_mouse_click(x, y, button, modifiers, nth)

class FileList(List):
    def __init__(self, parent, filter_func=None, on_file_dbl_click=None, **kwargs):
        self._filter_func = filter_func
        super(FileList, self).__init__(parent, **kwargs)

        self._directory = ""
        self.Directory = "/"
        self._on_file_dbl_click = on_file_dbl_click

    def _file_dbl_click(self, sender):
        if self._on_file_dbl_click:
            self._on_file_dbl_click(self)

    @property
    def Directory(self):
        return self._directory

    @Directory.setter
    def Directory(self, value):
        value = os.path.abspath(value)
        if self._directory == value:
            return

        self._directory = value
        self.clear()

        FileListItem(self, os.path.join(value, '..'))

        for filename in os.listdir(value):
            fullpath = os.path.join(value, filename)
            if self._filter_func and not stat.S_ISDIR(os.stat(fullpath).st_mode):
                if not self._filter_func(fullpath):
                    continue
            FileListItem(self, fullpath)

        self._items.sort()

        self._invalidate_alignment()

class FilePicker(Window):
    @classmethod
    def pick_file(cls, root, select_callback,
                  start_at=os.path.expanduser("~"),
                  filter_func=None,
                  **kwargs):
        def on_closed(sender):
            select_callback(sender.ModalResult)

        instance = cls(None,
                       start_at=start_at,
                       filter_func=filter_func,
                       on_modal_close=on_closed,
                       **kwargs)
        root.show_modal_window(instance)

    def __init__(self, parent,
                 start_at=os.path.expanduser("~"),
                 filter_func=None,
                 caption="Open file…",
                 **kwargs):
        super(FilePicker, self).__init__(parent, **kwargs)
        self.filelist = FileList(self,
                                 filter_func=filter_func,
                                 on_file_dbl_click=self._file_dbl_click)
        self.filelist.Directory = start_at
        self.Caption = caption
        self.AbsoluteRect.Width = 512
        self.AbsoluteRect.Height = 384

        buttonbar = HBox(self)
        buttonbar.StyleClasses.add("buttons")
        self._cancel = Button(buttonbar,
                              caption="Cancel",
                              on_click=self._do_cancel)
        self._open = Button(buttonbar,
                            caption="Open",
                            on_click=self._do_open)

    def _file_dbl_click(self, sender):
        if not sender.SelectedItem:
            return
        self.ModalResult = sender.SelectedItem._path

    def _do_cancel(self):
        pass

    def _do_open(self):
        pass


CSS.Minilanguage.ElementNames().register_widget_class(FileList)
