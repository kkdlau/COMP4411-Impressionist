/**
 *
 * @file an OS-generic header file for including openGL / GL.
 */

#if !defined(__GL__INC_H__)
#define __GL__INC_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#endif // __GL__INC_H__
