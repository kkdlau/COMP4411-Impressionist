//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "Image.hpp"
#include "ImpBrush.h"
#include <stdlib.h>

class ImpressionistDoc;

class PaintView : public Fl_Gl_Window {
public:
  Point line_start;
  Point line_end;
  Image prev;
  Image cur;

  PaintView(int x, int y, int w, int h, const char *l);
  void draw();
  int handle(int event);

  void refresh();

  void resizeWindow(int width, int height);

  void SaveCurrentContent();

  void RestoreContent();

  void draw_line(GLubyte r, GLubyte g, GLubyte b);

  void save_current_to(Image &img);
  void set_current_img(Image &img);

  ImpressionistDoc *m_pDoc;

private:
  GLvoid *m_pPaintBitstart;
  int m_nDrawWidth, m_nDrawHeight, m_nStartRow, m_nEndRow, m_nStartCol,
      m_nEndCol, m_nWindowWidth, m_nWindowHeight;
};

#endif