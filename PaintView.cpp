//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "Paintview.h"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

#define LEFT_MOUSE_DOWN 1
#define LEFT_MOUSE_DRAG 2
#define LEFT_MOUSE_UP 3
#define RIGHT_MOUSE_DOWN 4
#define RIGHT_MOUSE_DRAG 5
#define RIGHT_MOUSE_UP 6

#ifndef WIN32
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

static int eventToDo;
static int isAnEvent = 0;
static Point coord;

PaintView::PaintView(int x, int y, int w, int h, const char *l)
    : Fl_Gl_Window(x, y, w, h, l) {
  m_nWindowWidth = w;
  m_nWindowHeight = h;
}

void abort_event(int &event, ImpressionistDoc &doc) {
  StrokeDirection d = doc.m_pUI->get_direction();
  if (d != SLIDER_RIGHT_MOUSE) {
    // abort right click event
    if (event >= RIGHT_MOUSE_DOWN && event <= RIGHT_MOUSE_UP)
      event = 0;
  }
}

void PaintView::draw() {
#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

  if (!valid()) {

    glClearColor(0.7f, 0.7f, 0.7f, 1.0);

    // We're only using 2-D, so turn off depth
    glDisable(GL_DEPTH_TEST);

    ortho();

    glClear(GL_COLOR_BUFFER_BIT);
  }

  Point scrollpos; // = GetScrollPosition();
  scrollpos.x = 0;
  scrollpos.y = 0;

  m_nWindowWidth = w();
  m_nWindowHeight = h();

  int drawWidth, drawHeight;
  drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
  drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

  int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
  if (startrow < 0)
    startrow = 0;

  m_pPaintBitstart = m_pDoc->m_ucPainting +
                     3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

  m_nDrawWidth = drawWidth;
  m_nDrawHeight = drawHeight;

  m_nStartRow = startrow;
  m_nEndRow = startrow + drawHeight;
  m_nStartCol = scrollpos.x;
  m_nEndCol = m_nStartCol + drawWidth;

  if (m_pDoc->m_ucPainting && !isAnEvent) {
    RestoreContent();
  }

  if (m_pDoc->m_ucPainting && isAnEvent) {
    // Clear it after processing.
    isAnEvent = 0;

    abort_event(eventToDo, *m_pDoc);

    Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
    Point target(coord.x, m_nWindowHeight - coord.y);

    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      save_current_to(prev);

      m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      break;
    case LEFT_MOUSE_DRAG: {
      m_pDoc->m_pCurrentBrush->BrushMove(source, target);
      OriginalView &view = *pDoc->m_pUI->m_origView;
      view.set_cursor(target);
      break;
    }
    case LEFT_MOUSE_UP: {
      m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
      OriginalView &view = *pDoc->m_pUI->m_origView;
      view.hide_cusor();

      SaveCurrentContent();
      RestoreContent();
      break;
    }
    case RIGHT_MOUSE_DOWN: {
      line_start = source;
      line_end = source;
      break;
    }
    case RIGHT_MOUSE_DRAG: {
      line_end = target;
      RestoreContent();
      draw_line(RED_COLOR);

      break;
    }
    case RIGHT_MOUSE_UP: {
      m_pDoc->m_pUI->setAngle((int)rad_to_deg(target / line_start));
      RestoreContent();
      break;
    }

    default: {
      printf("Unknown event!!\n");
      break;
    }
    }
  }

  glFlush();

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA
}

int PaintView::handle(int event) {
  switch (event) {
  case FL_ENTER:
    redraw();
    break;
  case FL_PUSH:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_DOWN;
    else
      eventToDo = LEFT_MOUSE_DOWN;
    isAnEvent = 1;
    redraw();
    break;
  case FL_DRAG:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_DRAG;
    else
      eventToDo = LEFT_MOUSE_DRAG;
    isAnEvent = 1;
    redraw();
    break;
  case FL_RELEASE:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_UP;
    else
      eventToDo = LEFT_MOUSE_UP;
    isAnEvent = 1;
    redraw();
    break;
  case FL_MOVE:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    break;
  default:
    return 0;
    break;
  }

  return 1;
}

void PaintView::refresh() { redraw(); }

void PaintView::resizeWindow(int width, int height) {
  resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent() {
  // Tell openGL to read from the front buffer when capturing
  // out paint strokes
  glReadBuffer(GL_FRONT_AND_BACK);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

  glReadPixels(0, m_nWindowHeight - m_nDrawHeight, m_nDrawWidth, m_nDrawHeight,
               GL_RGB, GL_UNSIGNED_BYTE, m_pPaintBitstart);
}

void PaintView::RestoreContent() {
  glDrawBuffer(GL_BACK);

  glClear(GL_COLOR_BUFFER_BIT);

  glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
  glDrawPixels(m_nDrawWidth, m_nDrawHeight, GL_RGB, GL_UNSIGNED_BYTE,
               m_pPaintBitstart);

  //	glDrawBuffer(GL_FRONT);
}

void PaintView::draw_line(GLubyte r, GLubyte g, GLubyte b) {

  glLineWidth(3.0f);
  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINES);
  const GLubyte color[3] = {r, g, b};
  glColor3ubv(color);
  gl_set_point(line_start);
  gl_set_point(line_end);
  glEnd();

  pDoc->force_update_canvas();
}

void PaintView::save_current_to(Image &img) {
  img.set(m_pDoc->m_ucPainting, m_nDrawWidth, m_nDrawHeight);
}

void PaintView::set_current_img(Image &img) {
  cur = img;
  m_pDoc->m_ucPainting = cur.raw_fmt();
  refresh();
}
