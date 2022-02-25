//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//
#include <algorithm>
#include <vector>
#include <random>

#include "ImpBrush.h"
#include "Paintview.h"
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
    /*
  StrokeDirection d = pDoc->m_pUI->get_direction();
  if (d != SLIDER_RIGHT_MOUSE) {
    // abort right click event
    if (event >= RIGHT_MOUSE_DOWN && event <= RIGHT_MOUSE_UP)
      event = 0;
    if (!cur.valid_point(p.x, p.y)) {
      debugger("out-of-boundary");
      event = 0;
    }
  } */
}

void PaintView::draw() {
    finish_painting_flag = false;
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
    restore_content(cur.raw_fmt()); // painting
    if (auto_paint_flag) {
        auto_paint_flag = false;
        auto_paint();
    }
    else if (multires_paint_flag) {
        multires_paint_flag = false;
        multires_paint(); 
    }
    save_content(cur.raw_fmt());
    if (pDoc->app_mode == IMAGE)
        restore_content(overlay_image.raw_fmt()); // user loaded
  }

  if (cur.bytes.size() && isAnEvent) {
    // Clear it after processing.
    isAnEvent = 0;

    Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
    Point target(coord.x, m_nWindowHeight - coord.y);

    if (eventToDo) {
      restore_content(cur.raw_fmt());
    }
    // 
    // This is the event handler

    //printf("\n\non draw e: %d\n\n", eventToDo);
    fflush(stdout);
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      prev = cur; // for backup
      // restore_content(cur.raw_fmt());
      cur_brush.BrushBegin(source, target);
      save_content(cur.raw_fmt());

      break;
    case LEFT_MOUSE_DRAG: {
      cur_brush.BrushMove(source, target);
      org_view.set_cursor(target);
      save_content(cur.raw_fmt());
      break;
    }
    case LEFT_MOUSE_UP: {
      cur_brush.BrushEnd(source, target);
      save_content(cur.raw_fmt());
      restore_content(cur.raw_fmt());
      break;
    }
    case RIGHT_MOUSE_DOWN: {
      line_start = source;
      line_end = source;
      printf("\n\nmouse down\n\n");
      fflush(stdout);
      break;
    }
    case RIGHT_MOUSE_DRAG: {
      line_end = target;
      restore_content(cur.raw_fmt());
      draw_line(RED_COLOR);

      break;
    }
    case RIGHT_MOUSE_UP: {
        printf("Angle set is %f\n", rad_to_deg(target / line_start));
        printf("any diffference? %f\n", rad_to_deg(line_end / line_start));
       m_pDoc->m_pUI->setAngle((int)rad_to_deg(target / line_start));
      restore_content(cur.raw_fmt());
      printf("\n\nmouse up %d\n\n", (int)rad_to_deg(target / line_start));
      fflush(stdout);
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

    // render overlay content
    // restore_content(overlay_image.raw_fmt());
  }

  glFlush();

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA

  finish_painting_flag = true;
}

int PaintView::handle(int event) {
    //printf("\n\n event: %d\n\n", event);
    if (pDoc != NULL && pDoc->app_mode == VIDEO) {
        // dont handle any mouse event during video mode
        return 1;
    }
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
    printf("num mouse: %d", Fl::event_button());
    fflush(stdout);
    if (Fl::event_button() > 1) {
        Point target(coord.x, m_nEndRow - coord.y);
        m_pDoc->m_pUI->setAngle((int)rad_to_deg(target / line_start));
        eventToDo = RIGHT_MOUSE_UP;
        printf("fire right mouse up");
    }
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

void PaintView::refresh() { 
    finish_painting_flag = false;
    redraw(); }

void PaintView::resizeWindow(int width, int height) {
  resize(x(), y(), width, height);
}

void PaintView::save_content(GLvoid *ptr) {
  glReadBuffer(GL_FRONT_AND_BACK);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, cur.width);

  glReadPixels(0, m_nWindowHeight - cur.height, cur.width, cur.height, GL_RGBA,
               GL_UNSIGNED_BYTE, ptr);
}

void PaintView::restore_content(GLvoid *ptr) {
  glDrawBuffer(GL_FRONT_AND_BACK);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
  overlay_image = pDoc->m_pUI->m_origView->original_img;
  overlay_image.set_alpha(pDoc->m_pUI->getTransparency());
  refresh();
}

void PaintView::auto_paint(int s, short res) {
  ImpBrush &cur_brush = *m_pDoc->m_pCurrentBrush;
  int spacing = (s > 0)? s : m_pDoc->getSpacing();
  bool randomize = (m_pDoc->getAutoPaintRandomize() == 1);

  // randomize x and y
  std::vector<int> rows;
  std::random_device rd;
  std::mt19937 g(rd());
  for (int i = 1; i < cur.width; i += spacing)
    rows.push_back(i);
  std::shuffle(rows.begin(), rows.end(), g);
  std::vector<int> cols;
  for (int j = 1; j < cur.height; j += spacing)
    cols.push_back(j);
  std::shuffle(cols.begin(), cols.end(), g);

  int x = 0, y = 0;
  int counter = 0;
  bool start = true;
  Point source(x, y);
  Point target(x, y);
  for (std::vector<int>::iterator x = rows.begin(); x != rows.end(); ++x) {
    for (std::vector<int>::iterator y = cols.begin(); y != cols.end(); ++y) {
      source.x = *x + m_nStartCol;
      source.y = m_nEndRow - *y;
      target.x = *x;
      target.y = m_nWindowHeight - *y;

      if (start) {
        cur_brush.BrushBegin(source, target);
        start = false;
      } else
        cur_brush.BrushMove(source, target, randomize);
      // save_content(cur.raw_fmt());
      counter++;
      if (counter % 20 == 0)
        std::shuffle(cols.begin(), cols.end(), g);
    }
  }
  cur_brush.BrushEnd(source, target);
  printf("Finished autopainting\n");
  printf("%d %d %d %d %d\n", cur.width, cur.height, m_nStartCol, m_nEndRow, m_nWindowHeight);
}

void PaintView::multires_paint() {
    int brush_radii[3]{ 8, 4, 2 };
    for (int ind = 0; ind < 3; ind++) {
        // create reference image
        Image refImage = cur;
        ImageUtils::applyBlurFilter(m_pDoc->m_pUI->m_origView->original_img, refImage, brush_radii[ind] + 1);
        paint_layer(refImage, brush_radii[ind]);
    }
}

void PaintView::paint_layer(Image& reference, int radius) {
    std::vector<Point> strokes;
    // from Impressionist Painterly implementation
    const int grid = radius; 
    int threshold = 100; 
    // calculate pointwise difference image 
    std::vector<float> diff;
    for (int i = 1; i < cur.width; i++) {
        for (int j = 1; j < cur.height; j++) {
            // calculate l2 distance btw reference and cur 
            Point source(i + m_nStartCol, m_nEndRow - j);
            auto pixel1 = reference(source.y, source.x);
            auto pixel2 = cur(source.y, source.x);
            diff.push_back(dist(pixel1, pixel2));
        }
    }
    // find points to set strokes 
    for (int i = 1; i < cur.width; i += grid / 2) {
        for (int j = 1; j < cur.height; j += grid / 2) {
            // sum the error in the region
            float area_error = 0;
            int max_k = 0, max_m = 0; // to get the arg max Point 
            float max_diff = 0;
            for (int k = -grid / 2; k <= grid / 2; ++k) {
                for (int m = -grid / 2; m <= grid / 2; ++m) {
                    if (i + k < 0 || i + k >= cur.width) continue;
                    else if (j + m < 0 || j + m >= cur.height) continue;
                    int x = i + k, y = j + m;
                    float cur_diff = diff[x * cur.width + y];
                    if (cur_diff > max_diff) {
                        max_diff = cur_diff;
                        max_k = k; max_m = m;
                    }
                    area_error += cur_diff;
                }
            }
            if (area_error > threshold) {
                Point source(i + max_k + m_nStartCol, m_nEndRow - j - max_m);
                strokes.push_back(source);
            }
        }
    }
    
    // randomize strokes 
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(strokes.begin(), strokes.end(), g);

    ImpBrush& cur_brush = *m_pDoc->m_pCurrentBrush;
    std::for_each(strokes.begin(), strokes.end(), [&](Point& source) {
        int i = source.x - m_nStartCol;
        int j = m_nEndRow - source.y;
        Point target(i, m_nWindowHeight - j);
        cur_brush.BrushBegin(source, target, radius);
        cur_brush.BrushEnd(source, target);
        });
}