/* 
 * texture.h - texture marshalling function
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef TEXTURE_H
#define TEXTURE_H

/* loads a texture from a png
 * file pointed by filename
 * and returns a handle to it
 */
GLuint load_texture_from_png(const char *filename, int *width, int *height);

#endif /* TEXTURE_H */
