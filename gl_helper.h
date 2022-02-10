#if !defined(__GL_HELPER__)
#define __GL_HELPER__

#include "gl_inc.hpp"
#include "impressionistDoc.h"
#include <functional>

using namespace std;

#ifdef PROJ_DEBUG
#define debugger(...)                                                          \
  printf(__VA_ARGS__);                                                         \
  printf("\n");                                                                \
  fflush(stdout);
#else
#define debugger(...)
#endif

/**
 * @brief variable for deciding give up drawing or not.
 *
 * Each time when you call gl_draw_shape, it always assume the drawing is
 * "legal".
 *
 * However, sometimes the drawing is considered as "illagel", for example, out
 * of boundary drawing.
 *
 * In this case, invalid is set to true so that the helper functions won't
 * respond to any drawing request.
 *
 * This variable is reset to false every time a new drawing is triggered.
 */

#define RED_COLOR 255, 0, 0
namespace GLHelper {
extern bool invalid;

extern ImpressionistDoc *pDoc;
extern ImpressionistUI *pUI;

static void inline set_ui(ImpressionistUI *ui) { pUI = ui; }

static void inline set_doc(ImpressionistDoc *doc) {
  pDoc = doc;
  set_ui(pDoc->m_pUI);
}

static void gl_draw_shape(int drawing_mode, function<void()> drawing_funcs) {
  invalid = false;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glBegin(drawing_mode);

  drawing_funcs();

  glEnd();
}

static void gl_set_point(double x, double y) {
  // if (pDoc && pDoc->outOfRegion(Point{(int)x, (int)y})) {
  //   invalid = true;
  //   debugger("drawing is disable.");
  // }
  // if (invalid)
  //   return;
  glVertex2d(x, y);
}

static void gl_set_point(const Point &p) { gl_set_point(p.x, p.y); }

static float rad_to_deg(const float rad) { return rad / (2 * M_PI) * 360; }

} // namespace GLHelper

#endif // __GL_HELPER__
