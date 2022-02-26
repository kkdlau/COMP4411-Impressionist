#if !defined(__SCATTERED_POINT_BRUSH_H_)
#define __SCATTERED_POINT_BRUSH_H_
#include "PointBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

extern float frand();

class Line {
public:
  Point start;
  Point end;
  float slope;
  Line(const Point &s, const Point &e) : start{s}, end{e} {
    Point diff = start - end;
    slope = (float)diff.y / diff.x;
  }

  int y_transform(float x) { return end.y - slope * (end.x - x); }

  int x_transform(float y) { return end.y - (end.y - y) * slope; }

  Point transform(float x, float y) { return {x_transform(y), y_transform(x)}; }
  Point transform(float xy) { return {x_transform(xy), y_transform(xy)}; }
};

class ScatteredPointBrush : public PointBrush {
public:
  ScatteredPointBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : PointBrush(pDoc, name) {}
  
  void BrushBegin(const Point source, const Point target, int rad, GLubyte* color) {
    glPointSize(1);

    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target, GLubyte* color = nullptr, bool randomize = false) {
    ImpressionistDoc *pDoc = GetDocument();
    if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 ||
        source.y >= pDoc->m_nPaintHeight) {
        return;
    }
    const int half = pDoc->getSize() / 2;

    for (int x = -half; x <= half; x += 1) {
      for (int y = -half; y <= half; y += 1) {
          if (frand() <= 0.7) continue;
        Point rand = {x, y};
        PointBrush::BrushMove(rand + source, rand + target);
      }
    }

    pDoc->force_update_canvas();
  }

  void BrushEnd(const Point source, const Point target) {
      // do nothing so far
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

#endif // __SCATTERED_POINT_BRUSH_H_
