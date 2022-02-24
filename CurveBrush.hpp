#if !defined(__CURVE_BRUSH__)
#define __CURVE_BRUSH__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class CurveBrush : public ImpBrush {
  Point last;
  double radius;

public:
  CurveBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target, short resolution) {
    ImpressionistDoc *pDoc = GetDocument();
    last = target;
    const int width = pDoc->getWidth();
    glPointSize(width);
    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target, bool randomize=false) {
    ImpressionistDoc *pDoc = GetDocument();
    if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 ||
        source.y >= pDoc->m_nPaintHeight) {
        printf("Go back in\n"); // TODO - Remove
        return;
    }
    radius = 2 * pDoc->getSize();
    const int width = pDoc->getWidth();
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

    gl_draw_shape(GL_POINTS, [&] {
      SetColor(source);
      
      for (double i = M_PI / 4; i <= 3 * M_PI / 4; i += 0.01) {
        double angle = i + r;
        double dx = cos(angle) * radius - radius;
        double dy = sin(angle) * radius - radius;
        gl_set_point(target.x + dx, target.y + dy);
      }
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}

  void RandomizeAttributes() {
      glPointSize(irand(20));
      radius = irand(20);
  }

  void select() {
    ImpressionistDoc *pDoc = GetDocument();
    pDoc->m_pUI->m_BrushWidthSlider->activate();
    pDoc->m_pUI->m_BrushAngleSlider->activate();
    pDoc->m_pUI->m_StrokeDirection->activate();
    pDoc->m_pUI->m_BrushSizeSlider->activate();
    pDoc->m_pUI->m_BrushAlphaSlider->activate();
    pDoc->m_pUI->m_BrushBlurSlider->deactivate();
    pDoc->m_pUI->m_ColorBlending->activate();
    pDoc->m_pUI->m_MultiResPaint->deactivate();
  }
};

#endif // __CIRCLE_BRUSH__
