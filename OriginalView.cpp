//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "originalview.h"
#include "VideoUtils.hpp"
#include "gl_helper.h"
#include "impressionist.h"
#include "impressionistDoc.h"
#include <Fl/Fl.H>

#ifndef WIN32
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

using namespace GLHelper;

OriginalView::OriginalView(int x, int y, int w, int h, const char *l)
    : Fl_Gl_Window(x, y, w, h, l), img{nullptr, 0, 0}, original_img{nullptr, 0,
                                                                    0} {
  m_nWindowWidth = w;
  m_nWindowHeight = h;
  f = NO_FLAG;
}

void OriginalView::draw() {
#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

  if (!valid()) {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0);

    // We're only using 2-D, so turn off depth
    glDisable(GL_DEPTH_TEST);

    ortho();

    // glClear(GL_COLOR_BUFFER_BIT);
  }

  // glClear(GL_COLOR_BUFFER_BIT);

  if (original_img.bytes.size() > 0) {
    // note that both OpenGL pixel storage and the Windows BMP format
    // store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
    // around with startrow.
    m_nWindowWidth = w();
    m_nWindowHeight = h();

    GLvoid *bitstart;

    // we are not using a scrollable window, so ignore it
    Point scrollpos; // = GetScrollPosition();
    scrollpos.x = scrollpos.y = 0;

    int drawWidth = min(m_nWindowWidth, original_img.width);
    int drawHeight = min(m_nWindowHeight, original_img.height);

    int startrow = max(0, original_img.height - (scrollpos.y + drawHeight));

    // just copy image to GLwindow conceptually
    glRasterPos2i(0, m_nWindowHeight - drawHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
    glDrawBuffer(GL_BACK);
    glDrawPixels(drawWidth, drawHeight, GL_RGBA, GL_UNSIGNED_BYTE,
                 original_img.raw_fmt());

    if ((f & DrawingFlag::DISSOLVE) == DrawingFlag::DISSOLVE) {
      f = f & ~DISSOLVE;
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glRasterPos2i(0, m_nWindowHeight - dissolve_target.height);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, dissolve_target.width);
      glDrawPixels(dissolve_target.width, dissolve_target.height, GL_RGBA,
                   GL_UNSIGNED_BYTE, dissolve_target.paint_byte());

      save_to_image(original_img);
    }

    if (show_cursor)
      render_cursor();
  }

  glFlush();

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA
  this->refresh();
}

void OriginalView::save_to_image(Image &image) {
  glReadBuffer(GL_FRONT_AND_BACK);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, image.width);

  glReadPixels(0, m_nWindowHeight - image.height, image.width, image.height,
               GL_RGBA, GL_UNSIGNED_BYTE, image.raw_fmt());
}

void OriginalView::restore_from_image(Image &image) {
  glDrawBuffer(GL_BACK);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glRasterPos2i(0, m_nWindowHeight - image.height);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, image.width);
  glDrawPixels(image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE,
               image.paint_byte());
}

void OriginalView::refresh() { redraw(); }

void OriginalView::resizeWindow(int width, int height) {
  resize(x(), y(), width, height);
}

void OriginalView::set_current_img(Image &img) {
  this->img = img;
  original_img = img;
  cursor_pos = Point{img.width / 2, img.height / 2};
  // m_pDoc->m_ucBitmap = this->original_img.raw_fmt();
  this->resizeWindow(original_img.width, original_img.height);
  this->refresh();
  // keep reading images from the video
  ImpressionistDoc &doc = *pDoc;
  if (doc.app_mode == ApplicationMode::VIDEO) {
    add_timeout(1, [&](void *data) {
      Image next_frame = VideoUtils::next_frame();
      set_current_img(next_frame);
    });
  }
}

void OriginalView::dissolve(Image &img) {
  img.crop(original_img.width, original_img.height);
  img.set_alpha(0.5);
  dissolve_target = img;
  f |= static_cast<int>(DrawingFlag::DISSOLVE);
  this->refresh();
}

void OriginalView::render_cursor() {
  Point top_left = cursor_pos.shift_x(-3).shift_y(-3);
  Point bottom_right = cursor_pos.shift_x(3).shift_y(3);

  top_left = img.clip(top_left);
  bottom_right = img.clip(bottom_right);

  glPointSize(1);
  GLubyte c[3] = {255, 0, 0};

  glColor3ubv(c);

  gl_draw_shape(GL_POINTS, [&] {
    img.for_range_pixel(top_left, bottom_right,
                        [&](int y, int x) { gl_set_point(x, y); });
  });
}

void OriginalView::set_cursor(const Point &p) {
  cursor_pos = p;
  show_cursor = true;
  this->refresh();
}

void OriginalView::hide_cusor() {
  show_cursor = false;
  this->refresh();
}
