#if !defined(__SCATTERED_POINT_BRUSH_H_)
#define __SCATTERED_POINT_BRUSH_H_
#include "PointBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

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

  void BrushBegin(const Point source, const Point target) {
    glPointSize(1);

    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target) {
    ImpressionistDoc *pDoc = GetDocument();
    const int half = pDoc->getSize() / 2;
    for (float x = -half; x <= half; x += frand() * half) {
      for (float y = -half; y <= half; y += frand() * half) {
        Point rand = {(int)x, (int)y};
        PointBrush::BrushMove(source, rand + target);
      }
    }
  }
};

#endif // __SCATTERED_POINT_BRUSH_H_
