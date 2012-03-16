#include <glew.h>
#include <png.h>

#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace Resources {

struct Image;
typedef boost::shared_ptr<Image> ImageHandle;

struct Image
{
    public:
        Image(GLvoid *pixelData,
            const GLsizei aWidth, const GLsizei aHeight,
            const GLenum aFormat, const GLenum aType);
        virtual ~Image();
    protected:
        GLvoid *_pixelData;
    public:
        const GLenum format, type;
        const GLsizei width, height;
    public:
        bool getIsValid() const;
        void dropData();
    public:
        void texImage2D(const GLenum target, const GLint level,
            const GLint internalFormat) const;
        void texSubImage2D(const GLenum target, const GLint level,
            const GLint internalFormat,
            const GLint x, const GLint y) const;
    public:
        static ImageHandle PNGImage(FILE *infile);
};

}
}
