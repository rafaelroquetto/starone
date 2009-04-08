#ifndef GL_UTIL_H_
#define GL_UTIL_H_

#include <GL/gl.h>

struct image;

GLuint
image_to_opengl_texture(const struct image *image);

#endif /* GL_UTIL_H_ */
