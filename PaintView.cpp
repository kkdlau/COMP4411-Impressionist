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
#define MOUSE_MOVE 7

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

void PaintView::abort_event(int &event, Point &p) {
  StrokeDirection d = pDoc->m_pUI->get_direction();
  debugger(p.toString());
  if (d != SLIDER_RIGHT_MOUSE) {
    // abort right click event
    if (event >= RIGHT_MOUSE_DOWN && event <= RIGHT_MOUSE_UP)
      event = 0;
    if (!cur.valid_point(p.x, p.y)) {
      debugger("out-of-boundary");
      event = 0;
    }
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

    // glClear(GL_COLOR_BUFFER_BIT);
  }

  Point scrollpos; // = GetScrollPosition();
  scrollpos.x = 0;
  scrollpos.y = 0;

  m_nWindowWidth = w();
  m_nWindowHeight = h();

  int drawWidth, drawHeight;
  drawWidth = min(m_nWindowWidth, cur.width);
  drawHeight = min(m_nWindowHeight, cur.height);
  m_nDrawHeight = drawHeight;
  m_nDrawWidth = drawWidth;

  int startrow = cur.height - (scrollpos.y + drawHeight);
  if (startrow < 0)
    startrow = 0;

  m_pPaintBitstart = cur.raw_fmt() + 3 * ((cur.width * startrow) + scrollpos.x);

  m_nStartRow = startrow;
  m_nEndRow = startrow + drawHeight;
  m_nStartCol = scrollpos.x;
  m_nEndCol = m_nStartCol + drawWidth;

  OriginalView &org_view = *pDoc->m_pUI->m_origView;
  ImpBrush &cur_brush = *m_pDoc->m_pCurrentBrush;

  if (cur.bytes.size() && !isAnEvent) {
    restore_content(cur.raw_fmt());
  }

  if (cur.bytes.size() && isAnEvent) {
    // Clear it after processing.
    isAnEvent = 0;

    Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
    Point target(coord.x, m_nWindowHeight - coord.y);

    abort_event(eventToDo, target);

    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      prev = cur; // for backup
      // restore_content(cur.raw_fmt());
      cur_brush.BrushBegin(source, target);
      save_content(cur.raw_fmt());

      break;
    case LEFT_MOUSE_DRAG: {
      // restore_content(cur.raw_fmt());
      // Image overlay = org_view.original_img;
      // overlay.set_alpha(0.99);
      cur_brush.BrushMove(source, target);
      org_view.set_cursor(target);
      // save_content(cur.raw_fmt());
      // restore_content(overlay.raw_fmt());
      break;
    }
    case LEFT_MOUSE_UP: {
      cur_brush.BrushEnd(source, target);
      org_view.hide_cusor();
      save_content(cur.raw_fmt());
      restore_content(cur.raw_fmt());
      break;
    }
    case RIGHT_MOUSE_DOWN: {
      line_start = source;
      line_end = source;
      break;
    }
    case RIGHT_MOUSE_DRAG: {
      line_end = target;
      restore_content(cur.raw_fmt());
      draw_line(RED_COLOR);

      break;
    }
    case RIGHT_MOUSE_UP: {
      m_pDoc->m_pUI->setAngle((int)rad_to_deg(target / line_start));
      restore_content(cur.raw_fmt());
      break;
    }

    case MOUSE_MOVE: {
      org_view.set_cursor(target);
    } break;
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
    eventToDo = MOUSE_MOVE;
    isAnEvent = 1;

    redraw();
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

void PaintView::save_content(GLvoid *ptr) {
  glReadBuffer(GL_FRONT_AND_BACK);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, cur.width);

  glReadPixels(0, m_nWindowHeight - cur.height, cur.width, cur.height, GL_RGBA,
               GL_UNSIGNED_BYTE, ptr);
}

void PaintView::restore_content(GLvoid *ptr) {
  glDrawBuffer(GL_BACK);

  // glClear(GL_COLOR_BUFFER_BIT);

  glRasterPos2i(0, m_nWindowHeight - cur.height);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, cur.width);
  glDrawPixels(cur.width, cur.height, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

  //	glDrawBuffer(GL_FRONT);
}

void PaintView::draw_line(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {

  glLineWidth(3.0f);
  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINES);
  const GLubyte color[4] = {r, g, b, a};
  glColor4ubv(color);
  gl_set_point(line_start);
  gl_set_point(line_end);
  glEnd();

  pDoc->force_update_canvas();
}

void PaintView::set_current_img(Image &img) {
  cur = img;
  // m_pDoc->m_ucPainting = cur.raw_fmt();
  refresh();
}

void PaintView::auto_paint() {
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

    ImpBrush& cur_brush = *m_pDoc->m_pCurrentBrush;
    int spacing = (m_pDoc->getSize() / 2) > 5? m_pDoc->getSize() / 2 : 5;
    int x = 0, y = 0;
    Point source(x, y);
    Point target(x, y);
    for (x = 1; x < cur.width; x += spacing) {
        for (y = 1; y < cur.height; y += spacing) {
            source.x = x + m_nStartCol;
            source.y = m_nEndRow - y;
            target.x = x;
            target.y = m_nWindowHeight - y;

            if (!cur.valid_point(target.y, target.x)) {
                printf("out-of-boundary\t"); // TODO - remove
                continue;
            }
            if (x == 1 && y == 1)
                cur_brush.BrushBegin(source, target);
            else
                cur_brush.BrushMove(source, target);
            save_content(cur.raw_fmt());
        }
        
    }
    cur_brush.BrushEnd(source, target);
    save_content(cur.raw_fmt());
    restore_content(cur.raw_fmt());
    glFlush();
    #ifndef MESA
    // To avoid flicker on some machines.
    glDrawBuffer(GL_BACK);
    #endif // !MESA
    printf("Finished autopainting");
}