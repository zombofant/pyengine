/**********************************************************************
File name: StreamOperators.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/

/* tt3d Stream operators */

Stream &operator<<(Stream &stream, const int8 value);
Stream &operator<<(Stream &stream, const int16 value);
Stream &operator<<(Stream &stream, const int32 value);
Stream &operator<<(Stream &stream, const int64 value);
Stream &operator<<(Stream &stream, const uint8 value);
Stream &operator<<(Stream &stream, const uint16 value);
Stream &operator<<(Stream &stream, const uint32 value);
Stream &operator<<(Stream &stream, const uint64 value);
Stream &operator<<(Stream &stream, const std::string &value);

Stream &operator>>(Stream &stream, int8 &value);
Stream &operator>>(Stream &stream, int16 &value);
Stream &operator>>(Stream &stream, int32 &value);
Stream &operator>>(Stream &stream, int64 &value);
Stream &operator>>(Stream &stream, uint8 &value);
Stream &operator>>(Stream &stream, uint16 &value);
Stream &operator>>(Stream &stream, uint32 &value);
Stream &operator>>(Stream &stream, uint64 &value);
