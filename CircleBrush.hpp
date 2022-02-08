#if !defined(__CIRCLE_BRUSH__)
#define __CIRCLE_BRUSH__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class CircleBrush : public ImpBrush {
public:
  CircleBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    ImpressionistUI *dlg = pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize((float)size);

    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const int radius = pDoc->getSize() / 2;

    // for (float r = 0, PI2 = M_PI * 2; r <= PI2; r += M_PI / 10.0) {
    //   gl_draw(GL_TRIANGLE_FAN, [&] {
    //     Point p = target + Point::zero().shift_x(radius).rotate(r);
    //     gl_set_point(target);
    //     gl_set_point(Point(p.x, target.y));
    //     gl_set_point(Point(p.x, p.y));
    //   });
    // }
    gl_draw(GL_POLYGON, [&] {
      SetColor(source);
      for (float r = 0, PI2 = M_PI * 2; r <= PI2; r += M_PI / 10.0) {
        Point p = Point(target) + Point::zero().shift_x(radius).rotate(r);
        gl_set_point(p);
      }
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}
};

#endif // __CIRCLE_BRUSH__
