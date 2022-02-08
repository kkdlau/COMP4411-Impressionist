#if !defined(__LINE_BRUSH_H__)
#define __LINE_BRUSH_H__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class LineBrush : public ImpBrush {
public:
  LineBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    ImpressionistUI *dlg = pDoc->m_pUI;

    int size = pDoc->getSize();

    glLineWidth(size);

    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const int half = pDoc->m_pUI->getWidth() / 2;
    const float r = pDoc->getRad();

    gl_draw_shape(GL_LINES, [&] {
      SetColor(source);
      Point p1 = target + Point::zero().shift_x(-half).rotate(r);
      Point p2 = target + Point::zero().shift_x(half).rotate(r);

      debugger(p1.toString());
      debugger(p2.toString());
      gl_set_point(p1);
      gl_set_point(p2);
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}
};

#endif // __LINE_BRUSH_H__
