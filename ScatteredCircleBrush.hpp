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
    const int num_points = 3;

    for (int i = 0; i < num_points; i++) {
      Point t = target + Point::rand(-half, half);
      CircleBrush::BrushMove(t, t);
    }
  }
};

#endif // __SCATTERED_CIRCLE_BRUSH_H_
