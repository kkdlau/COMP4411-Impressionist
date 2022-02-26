#if !defined(__ARC_BRUSH__)
#define __ARC_BRUSH__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class ArcBrush : public ImpBrush {
  Point last;
  double radius;

public:
  ArcBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target, int rad, GLubyte* color) {
    ImpressionistDoc *pDoc = GetDocument();
    last = target;
    const int width = pDoc->getWidth();
    glPointSize(width);
    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target, GLubyte* color = nullptr, bool randomize = false) {
    ImpressionistDoc *pDoc = GetDocument();
    if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 ||
        source.y >= pDoc->m_nPaintHeight) {
        printf("Go back in\n"); // TODO - Remove
        return;
    }
    radius = pDoc->getSize();
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
    float offset_x = cos(r) * radius;
    float offset_y = sin(r) * radius;
    Point center(target.x + offset_x, target.y + offset_y);

    gl_draw_shape(GL_POINTS, [&] {
      SetColor(source);
      
      for (double i = 0; i <= M_PI / 2; i += 0.01) {
        double angle = i + r;
        double dx = cos(angle) * radius;
        double dy = sin(angle) * radius;
        Point cur(center.x + dx, center.y + dy);
        cur = pDoc->clip(cur);
        gl_set_point(cur.x, cur.y);
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
    pDoc->m_pUI->m_BrushCurvatureSlider->deactivate();
  }
};

#endif // __ARC_BRUSH__
