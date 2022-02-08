#if !defined(__SCATTERED_CIRCLE_BRUSH_H_)
#define __SCATTERED_CIRCLE_BRUSH_H_
#include "PointBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <functional>

using namespace GLHelper;
using namespace std;

extern float frand();

class ScatteredCircleBrush : public PointBrush {
public:
  ScatteredCircleBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : PointBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target) {
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

  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const int half = pDoc->getSize() / 2;
    for (int x = -half; x <= half; x += 1) {
      for (int y = -half; y <= half; y += 1) {
        if (within_circle(x, y, half)) {
          if (frand() <= 0.7) continue;
          Point src = Point{x, y} + target;
          PointBrush::BrushMove(src, src);
        }
      }
    }
  }
};

#endif // __SCATTERED_CIRCLE_BRUSH_H_
