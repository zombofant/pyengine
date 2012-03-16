#include "Image.hpp"

#include <cassert>

namespace PyUni {
namespace Resources {

/* PyUni::Resources::Image */

Image::Image(GLvoid *pixelData, 
        const GLsizei aWidth, const GLsizei aHeight,
        const GLenum aFormat, const GLenum aType):
    _pixelData(pixelData),
    format(aFormat),
    type(aType),
    width(aWidth),
    height(aHeight)
{
    assert(_pixelData != 0);
}

Image::~Image()
{
    if (_pixelData)
        free(_pixelData);
}

void Image::dropData()
{
    if (!_pixelData)
        return;
    free(_pixelData);
}

bool Image::getIsValid() const
{
    return (_pixelData != 0);
}

void Image::texImage2D(const GLenum target, const GLint level,
    const GLint internalFormat) const
{
    assert(_pixelData != 0);
    glTexImage2D(target, level, internalFormat, width, height,
        0, format, type, _pixelData);
}

void Image::texSubImage2D(const GLenum target, const GLint level,
    const GLint internalFormat, const GLint x, const GLint y) const
{
    assert(_pixelData != 0);
    glTexSubImage2D(target, level, x, y, width, height, format, type,
        _pixelData);
}

ImageHandle Image::PNGImage(FILE *infile)
{
    // Check if its really a PNG image
    unsigned char header[8];
    fread(header, 1, 8, infile);
    if (!png_sig_cmp(header, 0, 8))
    {
        fseek(infile, -8, SEEK_CUR);
        return ImageHandle();
    }

    // Initialize PNG data structures
    png_structp data = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!data)
    {
        // TODO: Exception
        return ImageHandle();
    }

    png_infop info = png_create_info_struct(data);
    if (!info)
    {
        png_destroy_read_struct(&data, 0, 0);
        // TODO: Exception
        return ImageHandle();
    }

    // libpng .... exception handling
    if (setjmp(png_jmpbuf(data)))
    {
        // TODO: Exception
        png_destroy_read_struct(&data, &info, 0);
        return ImageHandle();
    }

    // initialize libpng io
    png_init_io(data, infile);
    png_set_sig_bytes(data, 8);

    // we go the low-level way. This gives us the opportunity to put the
    // image in a big buffer instead of having each row separate. we
    // need that anyways.
    png_read_info(data, info);

    // using png_get_* instead of png_get_IHDR to be safe against int
    // size problems.
    const GLsizei width = png_get_image_width(data, info);
    const GLsizei height = png_get_image_height(data, info);

    const png_byte color_type = png_get_color_type(data, info);
    const png_byte bit_depth = png_get_bit_depth(data, info);
    
    // we calculate row bytes on our own as we may apply a
    // transformation
    GLsizei channelBytes = 0;
    GLsizei channels = png_get_channels(data, info);

    GLenum format = 0, type = 0;

    bool alpha = false, color = false;

    // detect if we need transformations
    if ((PNG_COLOR_MASK_PALETTE & color_type) != 0)
    {
        color = true;
        alpha = (PNG_COLOR_MASK_ALPHA & color_type) != 0;
        
        channelBytes = 1;
        channels =(alpha?4:3);
        png_set_palette_to_rgb(data);
    }
    else if (bit_depth < 8)
    {
        color = false;
        alpha = ((PNG_COLOR_MASK_ALPHA & color_type) != 0);
        
        channelBytes = 1;
        channels =(alpha?4:3);
        png_set_expand_gray_1_2_4_to_8(data);
        format = GL_LUMINANCE;
    }
    else
    {
        channelBytes = bit_depth / 8;
        assert(channelBytes * 8 == bit_depth);

        color = (PNG_COLOR_MASK_COLOR & color_type) != 0;
        alpha = (PNG_COLOR_MASK_ALPHA & color_type) != 0;
    }
    if ((!alpha) && png_get_valid(data, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(data);
        alpha = true;
        channels += 1;
    }

    // screw network-byte-order
    if (bit_depth > 8)
        png_set_swap(data);

    // detect opengl format and type
    switch (color)
    {
        case true:
        {
            switch (alpha)
            {
                case true:
                    format = GL_RGBA;
                    break;
                case false:
                    format = GL_RGB;
                    break;
                default:
                    format = 0;
            }
            break;
        }
        case false:
        {
            switch (alpha)
            {
                case true:
                    format = GL_LUMINANCE_ALPHA;
                    break;
                case false:
                    format = GL_LUMINANCE;
                    break;
                default:
                    format = 0;
            }
            break;
        }
        default:
            format = 0;
    }
    switch (bit_depth)
    {
        case 8:
            type = GL_UNSIGNED_BYTE;
            break;
        case 16:
            type = GL_UNSIGNED_SHORT;
            break;
        default:
            type = 0;
    }
    assert(format != 0);
    assert(type != 0);
    
    const GLsizei rowbytes = (channels * channelBytes * width);
    
    // at least we should not be below what libpng expects
    {
        
        const GLsizei pngRowbytes = png_get_rowbytes(data, info);
        assert(rowbytes >= pngRowbytes);
    }

    // now lets get some memory
    GLvoid *pixelData = malloc(height * rowbytes);

    // setup row array for libpng
    png_bytep rowPointers[height];
    unsigned char *pngPixelData = (unsigned char *)pixelData;
    for (GLsizei i = 0; i < height; i++)
    {
        rowPointers[i] = pngPixelData;
        pngPixelData += rowbytes;
    }

    png_read_image(data, rowPointers);
    
    // we bluntly ignore end info
    png_destroy_read_struct(&data, &info, 0);

    // now we have all the data, put it into an Image
    return ImageHandle(new Image(pixelData, width, height, format, type));
}

}
}
