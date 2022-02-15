//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "originalview.h"
#include "impressionist.h"
#include "impressionistDoc.h"

#ifndef WIN32
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

OriginalView::OriginalView(int x, int y, int w, int h, const char *l)
    : Fl_Gl_Window(x, y, w, h, l), img{nullptr, 0, 0}, original_img{nullptr, 0,
                                                                    0} {
  m_nWindowWidth = w;
  m_nWindowHeight = h;
}

void OriginalView::draw() {
  if (!valid()) {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0);

    // We're only using 2-D, so turn off depth
    glDisable(GL_DEPTH_TEST);

    // Tell openGL to read from the front buffer when capturing
    // out paint strokes
    glReadBuffer(GL_FRONT);
    ortho();
  }

  glClear(GL_COLOR_BUFFER_BIT);

  if (img.bytes.size() > 0) {
    // note that both OpenGL pixel storage and the Windows BMP format
    // store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
    // around with startrow.
    m_nWindowWidth = w();
    m_nWindowHeight = h();

    int drawWidth, drawHeight;
    GLvoid *bitstart;

    // we are not using a scrollable window, so ignore it
    Point scrollpos; // = GetScrollPosition();
    scrollpos.x = scrollpos.y = 0;

    drawWidth = min(m_nWindowWidth, img.width);
    drawHeight = min(m_nWindowHeight, img.height);

    int startrow = img.height - (scrollpos.y + drawHeight);
    if (startrow < 0)
      startrow = 0;

    bitstart =
        img.raw_fmt() + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

    // just copy image to GLwindow conceptually
    glRasterPos2i(0, m_nWindowHeight - drawHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
    glDrawBuffer(GL_BACK);
    glDrawPixels(drawWidth, drawHeight, GL_RGBA, GL_UNSIGNED_BYTE, bitstart);
  }

  glFlush();
}

void OriginalView::refresh() { redraw(); }

void OriginalView::resizeWindow(int width, int height) {
  resize(x(), y(), width, height);
}

void OriginalView::set_current_img(Image &img) {
  this->img = img;
  original_img = img;
  m_pDoc->m_ucBitmap = this->original_img.raw_fmt();
  this->resizeWindow(original_img.width, original_img.height);
  this->refresh();
}

void OriginalView::set_cursor(const Point &p) {
  Image &orig = this->original_img;
  Image &new_img = this->img;
  new_img = orig;
  Point top_left = p.shift_x(-3).shift_y(-3);
  Point bottom_right = p.shift_x(3).shift_y(3);
  new_img.for_range_pixel(top_left, bottom_right, [&](int y, int x) {
    new_img.set_pixel(y, x, {RED_COLOR});
  });
  this->img = new_img;
  this->refresh();
}

void OriginalView::hide_cusor() { set_current_img(original_img); }
