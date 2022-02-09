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

    int size = pDoc->m_pUI->getSize();

    glPointSize((float)size);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const int radius = pDoc->getSize() / 2;

    gl_draw_shape(GL_POINTS, [&] {
      SetColor(source);
      gl_set_point(target);
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}
};

#endif // __CIRCLE_BRUSH__
