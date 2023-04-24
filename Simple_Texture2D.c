// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Simple_Texture2D.c
//
//    This is a simple example that draws a quad with a 2D
//    texture image. The purpose of this example is to demonstrate
//    the basics of 2D texturing
//
#include <stdlib.h>
#include "esUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image.h"
#include "stb_image_write.h"

const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

typedef struct {
    // Handle to a program object
    GLuint programObject;
    GLuint programObjectRect;

    // Sampler location
    GLint samplerLoc;
    GLint samplerLocRect;

    // Texture handle
    GLuint textureId;
    GLuint textureIdRect;
    GLuint fbo;

    int frameWidth;
    int frameHeight;

    int frameIndex;

} UserData;

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D(int* width, int* height)
{
    // Texture object handle
    GLuint textureId;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
    {
       255,   0,   0, // Red
         0, 255,   0, // Green
         0,   0, 255, // Blue
       255, 255,   0  // Yellow
    };

    // Use tightly packed data
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);

    int nrChannels;
    unsigned char* data = stbi_load("F:\\coding\\opengles3-book-master\\Chapter_9\\Simple_Texture2D\\ylj.jpg", width, height, &nrChannels, 0);
    if (data) {
        //¸³Öµ
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {

    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Load the texture
    //glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;

}

GLuint CreateSimpleTexture2D2()
{
    // Texture object handle
    GLuint textureId;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
    {
      255,   0,   0, // Red
        0, 255,   0, // Green
        0,   0, 255, // Blue
      255, 255,   0  // Yellow
    };

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;

}


///
// Initialize the shader and program object
//
int Init(ESContext* esContext)
{
    UserData* userData = esContext->userData;
    char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = a_position;               \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

    char fShaderStr[] =
        "#version 300 es                                     \n"
        "precision mediump float;                            \n"
        "in vec2 v_texCoord;                                 \n"
        "layout(location = 0) out vec4 outColor;             \n"
        "uniform sampler2D s_texture;                        \n"
        "uniform sampler2D s_textureRect;                    \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  outColor = texture( s_texture, v_texCoord ) + texture( s_textureRect, v_texCoord );      \n"
        "}                                                   \n";

    // Load the shaders and get a linked program object
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

    // Get the sampler location
    userData->samplerLoc = glGetUniformLocation(userData->programObject, "s_texture");
    userData->samplerLocRect = glGetUniformLocation(userData->programObject, "s_textureRect");

    // Load the texture
    userData->textureId = CreateSimpleTexture2D(&(userData->frameWidth), &(userData->frameHeight));
    //userData->textureId2 = CreateSimpleTexture2D2();

    char vShaderStrRect[] =
        "#version 300 es                          \n"
        "layout(location = 0) in vec4 vPosition;  \n"
        "void main()                              \n"
        "{                                        \n"
        "   gl_Position = vPosition;              \n"
        "}                                        \n";

    char fShaderStrRect[] =
        "#version 300 es                              \n"
        "precision mediump float;                     \n"
        "out vec4 fragColor;                          \n"
        "void main()                                  \n"
        "{                                            \n"
        "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
        "}                                            \n";
    "}                                                   \n";
    userData->programObjectRect = esLoadProgram(vShaderStrRect, fShaderStrRect);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glGenFramebuffers(1, &userData->fbo);
    return TRUE;
}

void SaveImage(int width, int height, int frameIndex)
{
    //glReadBuffer(GL_COLOR_ATTACHMENT0);
    GLint readType, readFormat;
    GLubyte* pixels;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &readType);
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &readFormat);
    unsigned int bytesPerPixel = 0;
    switch (readType) {
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            switch (readFormat) {
                case GL_RGBA:
                    bytesPerPixel = 4;
                    break;
                case GL_RGB:
                case GL_RGB_INTEGER:
                    bytesPerPixel = 3;
                    break;
                case GL_RG:
                case GL_RG_INTEGER:
                case GL_LUMINANCE_ALPHA:
                    bytesPerPixel = 2;
                    break;
                case GL_RED:
                case GL_RED_INTEGER:
                case GL_ALPHA:
                case GL_LUMINANCE:
                    bytesPerPixel = 1;
                    break;
                default:
                    // Undetected format/error
                    break;
            }
            break;
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
        case GL_INT:
            switch (readFormat) {
                case GL_RGBA:
                case GL_RGBA_INTEGER:
                    bytesPerPixel = 16;
                    break;
                case GL_RGB:
                case GL_RGB_INTEGER:
                    bytesPerPixel = 12;
                    break;
                case GL_RG:
                case GL_RG_INTEGER:
                    bytesPerPixel = 8;
                    break;
                case GL_RED:
                case GL_RED_INTEGER:
                case GL_DEPTH_COMPONENT:
                    bytesPerPixel = 4;
                    break;
                default:
                    // Undetected format/error
                    break;
            }
            break;
        case GL_HALF_FLOAT:
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            switch (readFormat) {
                case GL_RGBA:
                case GL_RGBA_INTEGER:
                    bytesPerPixel = 8;
                    break;
                case GL_RGB:
                case GL_RGB_INTEGER:
                    bytesPerPixel = 6;
                    break;
                case GL_RG:
                case GL_RG_INTEGER:
                    bytesPerPixel = 4;
                    break;
                case GL_RED:
                case GL_RED_INTEGER:
                    bytesPerPixel = 2;
                    break;
                default:
                    // Undetected format/error
                    break;
            }
            break;
        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV: // GL_DEPTH_STENCIL
            bytesPerPixel = 8;
            break;
            // GL_RGBA, GL_RGBA_INTEGER format
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_10F_11F_11F_REV: // GL_RGB format
        case GL_UNSIGNED_INT_5_9_9_9_REV: // GL_RGB format
        case GL_UNSIGNED_INT_24_8: // GL_DEPTH_STENCIL format
            bytesPerPixel = 4;
            break;
        case GL_UNSIGNED_SHORT_4_4_4_4: // GL_RGBA format
        case GL_UNSIGNED_SHORT_5_5_5_1: // GL_RGBA format
        case GL_UNSIGNED_SHORT_5_6_5: // GL_RGB format
            bytesPerPixel = 2;
            break;
        default:
            break;
            // Undetected type/error
    }
    pixels = (GLubyte*)malloc(width * height * bytesPerPixel);
    glReadPixels(0, 0, width, height, readFormat, readType, pixels);

    char buf[128];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "F:\\frame_%dX%d_%d.bmp",
        width, height, frameIndex);
    stbi_write_bmp(buf, width, height, bytesPerPixel, pixels);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext* esContext)
{
    UserData* userData = esContext->userData;
    GLfloat vVerticesRect[] = { 0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f,
                            0.0f, 0.5f, 0.0f
    };

    if (userData->textureIdRect == 0) {
        glGenTextures(1, &userData->textureIdRect);
        glBindTexture(GL_TEXTURE_2D, userData->textureIdRect);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, userData->frameWidth, userData->frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, userData->fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, userData->textureIdRect, 0);

    // Set the viewport
    glViewport(0, 0, userData->frameWidth, userData->frameHeight);

    //// Clear the color buffer
    //glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObjectRect);

    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVerticesRect);
    glEnableVertexAttribArray(0);
    glLineWidth(8);
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    if (userData->frameIndex < 1) {
        SaveImage(userData->frameWidth, userData->frameHeight, userData->frameIndex);
        ++userData->frameIndex;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*GLuint tmpFramebuffers;
    glGenFramebuffers(1, &tmpFramebuffers);
    glBindFramebuffer(GL_FRAMEBUFFER, tmpFramebuffers);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        userData->textureIdRect, 0);
    if (userData->frameIndex < 3) {
        SaveImage(userData->frameWidth, userData->frameHeight, userData->frameIndex);
        ++userData->frameIndex;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
    double vWidth = 1.0f;
    double vheight = 1.0f;
    if (userData->frameWidth / userData->frameHeight >= WIN_WIDTH / WIN_HEIGHT) {
        vheight = (double)userData->frameHeight / userData->frameWidth * WIN_WIDTH / WIN_HEIGHT;
    } else {
        vWidth = (double)userData->frameWidth / userData->frameHeight * WIN_HEIGHT / WIN_WIDTH;
    }
    GLfloat vVertices[] = { -vWidth, vheight, 0.0f,  // Position 0
                             0.0f,  0.0f,        // TexCoord 0 
                            -vWidth, -vheight, 0.0f,  // Position 1
                             0.0f,  1.0f,        // TexCoord 1
                             vWidth, -vheight, 0.0f,  // Position 2
                             1.0f,  1.0f,        // TexCoord 2
                             vWidth,  vheight, 0.0f,  // Position 3
                             1.0f,  0.0f         // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObject);

    // Load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT,
        GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT,
        GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind the texture
    //glActiveTexture ( GL_TEXTURE0 + 0 );
    //glBindTexture ( GL_TEXTURE_2D, userData->textureId );

    //// Set the sampler texture unit to 0
    //glUniform1i ( userData->samplerLoc, 0 );
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, userData->textureId);

    // Set the sampler texture unit to 0
    glUniform1i(userData->samplerLoc, 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, userData->textureIdRect);

    // Set the sampler texture unit to 0
    glUniform1i(userData->samplerLocRect, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    //GLfloat vVertices2[] = { 0.0f,  0.0f,        // TexCoord 0 
    //                         0.0f,  1.0f,        // TexCoord 2
    //                         1.0f,  1.0f,         // TexCoord 3
    //                         1.0f,  0.0f        // TexCoord 1
    //};
    ////GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    //// Set the viewport
    //glViewport(0, 0, esContext->width, esContext->height);

    //glClear ( GL_COLOR_BUFFER_BIT );

    //// Use the program object
    //glUseProgram(userData->programObject);

    //// Load the vertex position
    //// Load the texture coordinate
    //glVertexAttribPointer(2, 2, GL_FLOAT,
    //    GL_FALSE, 2 * sizeof(GLfloat), vVertices2);

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    //// Bind the texture
    ////glActiveTexture(GL_TEXTURE0 + 1);
    ////glBindTexture(GL_TEXTURE_2D, userData->textureId2);

    ////// Set the sampler texture unit to 0
    ////glUniform1i(userData->samplerLoc2, 1);

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
}

///
// Cleanup
//
void ShutDown(ESContext* esContext)
{
    UserData* userData = esContext->userData;

    // Delete texture object
    glDeleteTextures(1, &userData->textureId);

    // Delete program object
    glDeleteProgram(userData->programObject);
}


int esMain(ESContext* esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    memset(esContext->userData, 0, sizeof(UserData));
    esCreateWindow(esContext, "Simple Texture 2D", WIN_WIDTH, WIN_HEIGHT, ES_WINDOW_RGB);

    if (!Init(esContext)) {
        return GL_FALSE;
    }

    esRegisterDrawFunc(esContext, Draw);
    esRegisterShutdownFunc(esContext, ShutDown);

    return GL_TRUE;
}
