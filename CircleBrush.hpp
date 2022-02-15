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

    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const double radius = pDoc->getSize() / 2.0;

    gl_draw_shape(GL_POLYGON, [&] {
      SetColor(source);
      for (double i = 0; i <= 2 * M_PI; i += 0.1) {
        double dx = cos(i) * radius;
        double dy = sin(i) * radius;

        gl_set_point(target.x + dx, target.y + dy);
      }
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}
  void select() {
      ImpressionistDoc* pDoc = GetDocument();
      pDoc->m_pUI->m_BrushWidthSlider->deactivate();
      pDoc->m_pUI->m_BrushAngleSlider->deactivate();
      pDoc->m_pUI->m_StrokeDirection->deactivate();
      pDoc->m_pUI->m_BrushSizeSlider->activate();
      pDoc->m_pUI->m_BrushAlphaSlider->activate();
      pDoc->m_pUI->m_BrushBlurSlider->deactivate();
      pDoc->m_pUI->m_ColorBlending->activate();
  }
};

#endif // __CIRCLE_BRUSH__
