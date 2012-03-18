/**********************************************************************
File name: Font.hpp
This file is part of: Pythonic Universe

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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYUNI_RESOURCES_FONT_H
#define _PYUNI_RESOURCES_FONT_H

#include <unordered_map>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "IO/IO.hpp"
#include <boost/weak_ptr.hpp>

namespace PyUni {
namespace Resources {

enum FontStyle {
    FS_NONE = 0,
    FS_ITALIC = 1,
    FS_BOLD = 2
};

class FontFace;
typedef boost::shared_ptr<FontFace> FontFaceHandle;

typedef std::unordered_map<unsigned int, FontFaceHandle> FontFaceMap;

class FontFace
{
    public:
        FontFace(FT_Face face);
        virtual ~FontFace();
    private:
        FT_Face _face;
        const unsigned int _style;
    public:
        inline FT_Face getFace() const { return _face; };
        inline unsigned int getStyle() const { return _style; };
};

class FontLibrary;
typedef boost::shared_ptr<FontLibrary> FontLibraryHandle;

class Font
{
    public:
        Font(FontLibraryHandle fontLibrary, const float size);
        ~Font();
    private:
        FontLibraryHandle _library;
        const float _size;
        FontFaceMap _faceMap;
    public:
        FontFaceHandle get(unsigned int style);
        FontFaceHandle load(IStreamHandle input, int faceIndex);
};

typedef boost::shared_ptr<Font> FontHandle;
typedef std::pair<const std::string, const float> FontMapTuple;
typedef std::unordered_map<FontMapTuple, FontHandle> FontMap;
typedef boost::weak_ptr<FontLibrary> WeakFontLibraryHandle;

class FontLibrary
{
    public:
        FontLibrary(FT_Library _lib);
        ~FontLibrary();
    private:
        FT_Library _lib;
        FontMap _fontMap;
        WeakFontLibraryHandle _weak;
    public:
        FontHandle add(const std::string &name, const float size);
        FontHandle get(const std::string &name, const float size);
        FontFaceHandle getFontFace(const std::string &fontName,
            const float size, unsigned int style);
        inline FT_Library getFTLibrary() { return _lib; }
    public:
        static FontLibraryHandle create();
};

}
}

#endif
