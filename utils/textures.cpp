#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"
#include "utils/textures.h"

static short le_short(unsigned char *bytes)/*{{{*/
{
    return bytes[0] | ((char)bytes[1] << 8);
}/*}}}*/

GLuint loadTGATexture(const std::string& imagepath, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter, GLenum min_filter, bool anisotropy)/*{{{*/
{             
    /*!todo [Textures] exercice 1: load a texture from a file into the GPU as follows,
     *  1) Generate a texture location in GPU
     *  2) Make it active (bind it)
     *  3) Load the texture file into a CPU buffer 
     *     (using the read_tga routine from utils/textures.h)
     *  4) Send this data to the GPU (glTexImage2D)
     *  5) Set up the Wrap and filter parameters. 
     *     */

    // Create one OpenGL texture
    GLuint textureID=0;

    // Laoding in CPU
    int width, height, depth;

    const char * file = imagepath.c_str();
    char* data = read_tga(file, width, height, depth );

    // Uploading to GPU
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    // Return the ID of the texture we just created
    return textureID;
}/*}}}*/


GLuint loadTGACubemap( std::string* faceNames, GLenum* faceTypes)/*{{{*/
{
    /*!todo [Skybox] exercice 1: Create a cube map texture (GL_TEXTURE_CUBE_MAP target) from 6 file names
     *  1) Generate a texture location in GPU
     *  2) Make it active (bind it to the GL_TEXTURE_CUBE_MAP target)
     *  3) Load the 6 texture files into 6 CPU buffers 
     *     (using the read_tga)
     *  4) Send these data to the GPU (glTexImage2D)
     *  5) Set up the Wrap and filter parameters. 
     *     */
    // Create one OpenGL texture
    GLuint textureID=0;

    return textureID;
}/*}}}*/

char *read_tga(const char *filename, int& width, int& height, int& depth)/*{{{*/
{
    struct tga_header {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;
    int i, color_map_size;
    FILE *f;
    size_t read;
    char *pixels;

    f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24 and header.bits_per_pixel != 32) {
        fprintf(stderr, "%s is not a 24-bit or 32-bit uncompressed RGB tga file but %i\n", filename, header.bits_per_pixel);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    width = le_short(header.width); height = le_short(header.height);
    depth = (header.bits_per_pixel/8);
    unsigned int pixels_size = width * height * depth;
    pixels = new char[pixels_size];

    read = fread(pixels, 1, pixels_size, f);

    if (read != pixels_size) {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        delete [] pixels;
        return NULL;
    }

    return pixels;
}/*}}}*/

