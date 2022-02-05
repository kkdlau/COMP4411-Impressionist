#ifndef IMPRESSIONIST_H
#define IMPRESSIONIST_H
#define SAMPLE_SOLUTION
#include <FL/Fl.H>
#include <FL/gl.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "ImpBrush.h"

#define TRUE 1
#define FALSE 0

#ifdef WIN32
#define M_PI 3.1415926535F
#endif

#endif