//
// Created by selya on 13.11.2018.
//

#ifndef A2D_TEXTURE_H
#define A2D_TEXTURE_H

#include <a2d/core/macro.h>
#include <a2d/core/ref_counter.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#endif

#include <string>
#include <map>

namespace a2d {

DECLARE_SMART_POINTER(Texture)

class Texture : public ref_counter {
public:
    friend class Engine;

    const int width;
    const int height;

    Texture(int width, int height, const unsigned char *data, bool flip = false, bool mipmaps = false);
    ~Texture();

    void Bind(unsigned int texture_unit = 0) const;

    static void Unbind(unsigned int texture_unit = 0);

private:
    GLuint texture_id;
};

} //namespace a2d


#endif //A2D_TEXTURE_H
