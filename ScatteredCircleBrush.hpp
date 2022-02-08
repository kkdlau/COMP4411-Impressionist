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
    for (float x = -half; x <= half; x += frand() * half) {
      for (float y = -half; y <= half; y += frand() * half) {
        if (within_circle(x, y, half)) {
          debugger("draw");
          Point rand = {(int)x, (int)y};
          PointBrush::BrushMove(source, Point{(int)x, (int)y} + target);
        } else {
          debugger("gg");
        }
      }
    }
  }
};

#endif // __SCATTERED_CIRCLE_BRUSH_H_
