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

extern int irand(int max);
extern float frand();
class ImpressionistDoc;

class PaintView : public Fl_Gl_Window {
public:
  Point line_start;
  Point line_end;

  /**
   * @brief The image generated by last user painting.
   *
   */
  Image prev;
  /**
   * @brief The current image generated by user painting.
   */
  Image cur;

  Image painting_buffer;

  PaintView(int x, int y, int w, int h, const char *l);
  void draw();
  int handle(int event);

  void refresh();

  void resizeWindow(int width, int height);

  void save_content(GLvoid *ptr);

  void restore_content(GLvoid *ptr);

  void draw_line(GLubyte r, GLubyte g, GLubyte b, GLubyte a);

  void abort_event(int& e, Point& p);

  void set_current_img(Image &img);

  void auto_paint();

  ImpressionistDoc *m_pDoc;

private:
  GLvoid *m_pPaintBitstart;
  int m_nDrawWidth, m_nDrawHeight, m_nStartRow, m_nEndRow, m_nStartCol,
      m_nEndCol, m_nWindowWidth, m_nWindowHeight;
};

#endif