/**********************************************************************
File name: Font.cpp
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
#include "Font.hpp"
#include "FTIOWrapper.hpp"
#include "Misc/Exception.hpp"

namespace PyUni {
namespace Resources {

inline void catchFTError(FT_Error err)
{
    if (err != 0)
    {
        throw ExternalError("FreeType");
    }
}

/* PyUni::Resources::FontFace */

FontFace::FontFace(FT_Face face):
    _face(face),
    _style(face->style_flags)
{
    
}

FontFace::~FontFace()
{
    FT_Done_Face(_face);
}

/* PyUni::Resources::Font */

Font::Font(FontLibraryHandle fontLibrary, const float size):
    _library(fontLibrary),
    _size(size),
    _faceMap()
{
    
}

Font::~Font()
{

}

FontFaceHandle Font::get(unsigned int style)
{
    auto it = _faceMap.find(style);
    if (it != _faceMap.end())
        return _faceMap[style];
    else
        return FontFaceHandle();
}

FontFaceHandle Font::load(IStreamHandle input, int faceIndex)
{
    FT_Face face;
    catchFTError(FT_Open_Face(_library->getFTLibrary(), openAsStream(input), faceIndex, &face));
    try {
        catchFTError(FT_Set_Char_Size(face, (int)(_size*64), 0, 72, 72));
    } catch (...) {
        FT_Done_Face(face);
        throw;
    }
    FontFaceHandle handle = FontFaceHandle(new FontFace(face));
    _faceMap[handle->getStyle()] = handle;
    return handle;
}

/* PyUni::Resources::FontLibrary */

FontLibrary::FontLibrary(FT_Library lib):
    _lib(lib),
    _fontMap(),
    _weak()
{

}

FontLibrary::~FontLibrary()
{
    _fontMap.clear();
    FT_Done_FreeType(_lib);
}

FontHandle FontLibrary::add(const std::string &name, const float size)
{
    FontMapTuple tuple(name, size);
    auto it = _fontMap.find(tuple);
    if (it != _fontMap.end())
    {
        return it->second;
    }
    FontHandle font(new Font(_weak.lock(), size));
    _fontMap[tuple] = font;
    return font;
}

FontHandle FontLibrary::get(const std::string &name, const float size)
{
    auto it = _fontMap.find(FontMapTuple(name, size));
    if (it == _fontMap.end())
        return FontHandle();
    return it->second;
}

FontLibraryHandle FontLibrary::create()
{
    FT_Library lib = 0;
    catchFTError(FT_Init_FreeType(&lib));
    if (lib == 0)
        return FontLibraryHandle();
    return FontLibraryHandle(new FontLibrary(lib));
}

}
}
