//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>

#include "Image.hpp"
#include "ImpBrush.h"

class ImpressionistDoc;

enum DrawingFlag {
  NO_FLAG,
  DISSOLVE = 0b1 << 1,
};

class OriginalView : public Fl_Gl_Window {
public:
  OriginalView(int x, int y, int w, int h, const char *l);

  void draw();
  void refresh();

  void resizeWindow(int width, int height);

  void set_current_img(Image &img);

  void save_to_image(Image &image);
  void restore_from_image(Image &image);

  void dissolve(Image &img);

  void set_cursor(const Point &p);

  void render_cursor();

  void hide_cusor();


  ImpressionistDoc *m_pDoc;
  Image img;
  Image original_img;

  Image dissolve_target;

  Point cursor_pos;

  uint32_t f;

private:
  bool show_cursor;
  int m_nWindowWidth, m_nWindowHeight;
};

#endif