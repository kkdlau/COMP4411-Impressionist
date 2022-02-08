#if !defined(__SCATTERED_LINE_BRUSH_H_)
#define __SCATTERED_LINE_BRUSH_H_
#include "LineBrush.hpp"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <functional>

using namespace GLHelper;
using namespace std;

extern float frand();

class ScatteredLineBrush : public LineBrush {
public:
  ScatteredLineBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : LineBrush(pDoc, name) {}

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
        Point src = Point{(int)x, (int)y} + target;
        LineBrush::BrushMove(src, src);
      }
    }
  }
};

#endif // __SCATTERED_LINE_BRUSH_H_
