#if !defined(__LINE_BRUSH_H__)
#define __LINE_BRUSH_H__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

class LineBrush : public ImpBrush {
  Point last;

public:
  LineBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    ImpressionistUI *dlg = pDoc->m_pUI;

    last = target;
    int size = pDoc->getSize();
    int width = pDoc->getWidth();

    glLineWidth(width);

    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target, bool randomize=false) {
    ImpressionistDoc *pDoc = GetDocument();
    const int half = pDoc->m_pUI->getSize() / 2;
    float r = pDoc->getRad();
    switch (pDoc->m_pUI->get_direction()) {
    case GRADIENT_DIRECTION: {
      auto result = ImageUtils::sobel(pDoc->m_pUI->m_origView->original_img,
                                      target.y, target.x);
      r = get<2>(result);
    } break;

    case BRUSH_DIRECTION: {
      r = target / last;
    } break;
    }

    if (isnan(r))
      return;
    if (randomize && frand() >= 0.75)
        RandomizeAttributes();

    gl_draw_shape(GL_LINES, [&] {
      SetColor(source);
      Point p1 = target + Point::zero().shift_x(-half).rotate(r);
      Point p2 = target + Point::zero().shift_x(half).rotate(r);

      gl_set_point(p1);
      gl_set_point(p2);
    });

    pDoc->force_update_canvas();

    last = target;
  }
  void BrushEnd(const Point source, const Point target) {}

  void select() {
      ImpressionistDoc* pDoc = GetDocument();
      pDoc->m_pUI->m_BrushWidthSlider->activate();
      pDoc->m_pUI->m_BrushAngleSlider->activate();
      pDoc->m_pUI->m_StrokeDirection->activate();
      pDoc->m_pUI->m_BrushSizeSlider->activate();
      pDoc->m_pUI->m_BrushAlphaSlider->activate();
      pDoc->m_pUI->m_BrushBlurSlider->deactivate();
      pDoc->m_pUI->m_ColorBlending->activate();
  }

  void RandomizeAttributes() {
      glLineWidth(40);
  }
};

#endif // __LINE_BRUSH_H__
