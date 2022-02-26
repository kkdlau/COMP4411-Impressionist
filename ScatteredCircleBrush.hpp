#if !defined(__SCATTERED_CIRCLE_BRUSH_H_)
#define __SCATTERED_CIRCLE_BRUSH_H_
#include "CircleBrush.hpp"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <functional>

using namespace GLHelper;
using namespace std;

extern float frand();

class ScatteredCircleBrush : public CircleBrush {
public:
  ScatteredCircleBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : CircleBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target, int rad, GLubyte* color) {
    glPointSize(1);

    BrushMove(source, target);
  }

  bool within_circle(float x, float y, float r) {
    float dx = abs(x);
    if (dx > r)
      return false;
    float dy = abs(y);
    if (dy > r)
      return false;
    if (dx + dy <= r)
      return true;
    return (dx * dx + dy * dy <= r * r);
    return true;
  }

  void BrushMove(const Point source, const Point target, GLubyte* color = nullptr, bool randomize = false) {
    ImpressionistDoc *pDoc = GetDocument();
    if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 ||
        source.y >= pDoc->m_nPaintHeight) {
        return;
    }
    const int half = pDoc->getSize() / 2;
    const int num_points = 3;

    for (int i = 0; i < num_points; i++) {
      Point t = target + Point::rand(-half, half);
      CircleBrush::BrushMove(t, t);
    }
  }

  void select() {
      ImpressionistDoc* pDoc = GetDocument();
      pDoc->m_pUI->m_BrushWidthSlider->deactivate();
      pDoc->m_pUI->m_BrushAngleSlider->deactivate();
      pDoc->m_pUI->m_StrokeDirection->deactivate();
      pDoc->m_pUI->m_BrushSizeSlider->activate();
      pDoc->m_pUI->m_BrushAlphaSlider->activate();
      pDoc->m_pUI->m_BrushBlurSlider->deactivate();
      pDoc->m_pUI->m_ColorBlending->activate();
      pDoc->m_pUI->m_MultiResPaint->deactivate();
      pDoc->m_pUI->m_BrushCurvatureSlider->deactivate();
  }
};

#endif // __SCATTERED_CIRCLE_BRUSH_H_
