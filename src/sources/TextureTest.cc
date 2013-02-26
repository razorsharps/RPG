#include "../headers/TextureTest.h"

using namespace Ymir;

TextureTest::TextureTest(Bitmap& bitmap, GLint minMagFilter, GLint wrapMode) : _width(bitmap.width()), _height(bitmap.height())
{
    glGenTextures(1, &_object);
    glBindTexture(GL_TEXTURE_2D, _object);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    GLenum format;
    switch(bitmap.format())
    {
        case Bitmap::Format::Grayscale:
            format = GL_LUMINANCE;
            break;
        case Bitmap::Format::GrayscaleAlpha:
            format = GL_LUMINANCE_ALPHA;
            break;
        case Bitmap::Format::RGB:
            format = GL_RGB;
            break;
        case Bitmap::Format::RGBA:
            format = GL_RGBA;
            break;
    }
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 (GLsizei)bitmap.width(),
                 (GLsizei)bitmap.height(),
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 bitmap.pixelBuffer());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureTest::TextureTest(TextureTest&& other) : _object(other._object)
{
    other._object = 0;
}

TextureTest::~TextureTest()
{
    if(_object)
        glDeleteTextures(1, &_object);
}

TextureTest& TextureTest::operator=(TextureTest&& other)
{
    if(_object)
        glDeleteTextures(1, &_object);
    
    _object = other._object;
    other._object = 0;
    return *this;
}

GLuint TextureTest::object() const
{
    return _object;
}

GLfloat TextureTest::width() const
{
    return _width;
}

GLfloat TextureTest::height() const
{
    return _height;
}

void TextureTest::bind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _object);
}

void TextureTest::unbind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}