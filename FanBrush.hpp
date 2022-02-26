#if !defined(__FAN_BRUSH__)
#define __FAN_BRUSH__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class FanBrush : public ImpBrush {
  Point last;
  double radius;

public:
  FanBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target, int rad,
                  GLubyte *color) {
    ImpressionistDoc *pDoc = GetDocument();
    last = target;
    radius = (rad > 0) ? rad : pDoc->getSize();
    BrushMove(source, target, color);
  }

  void BrushMove(const Point source, const Point target,
                 GLubyte *color = nullptr, bool randomize = false) {
    ImpressionistDoc *pDoc = GetDocument();
    auto &img = pDoc->m_pUI->m_paintView->cur;
    if (source.x <= 0 || source.x >= img.width || source.y <= 0 ||
        source.y >= img.height) {
      printf("Go back in\n"); // TODO - Remove
      return;
    }
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

    gl_draw_shape(GL_TRIANGLE_FAN, [&] {
      if (color) {
        glColor4ubv(color);
      } else
        SetColor(source);
      gl_set_point(target.x, target.y);
      for (double i = M_PI / 6; i <= 5 * M_PI / 6; i += 0.1) {
        double angle = i + r;
        double dx = cos(angle) * radius;
        double dy = sin(angle) * radius;
        Point point = target + Point{(int)dx, (int)dy};
        point = pDoc->clip(point);
        gl_set_point(target.x + dx, target.y + dy);
      }
    });

    pDoc->force_update_canvas();
  }
  void BrushEnd(const Point source, const Point target) {}

  void RandomizeAttributes() {
    ImpressionistDoc *pDoc = GetDocument();
    radius = irand(pDoc->getSize() + 5);
  }

  void select() {
    ImpressionistDoc *pDoc = GetDocument();
    pDoc->m_pUI->m_BrushWidthSlider->deactivate();
    pDoc->m_pUI->m_BrushAngleSlider->activate();
    pDoc->m_pUI->m_StrokeDirection->activate();
    pDoc->m_pUI->m_BrushSizeSlider->activate();
    pDoc->m_pUI->m_BrushAlphaSlider->activate();
    pDoc->m_pUI->m_BrushBlurSlider->deactivate();
    pDoc->m_pUI->m_ColorBlending->activate();
    pDoc->m_pUI->m_MultiResPaint->activate();
    pDoc->m_pUI->m_BrushCurvatureSlider->deactivate();
  }
};

#endif // __CIRCLE_BRUSH__
