#if !defined(__GRADIENT_BRUSH_H__)
#define __GRADIENT_BRUSH_H__
#include "LineBrush.hpp"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

class GradientBrush : public LineBrush {
public:
  GradientBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : LineBrush(pDoc, name) {}

  void BrushBegin(const Point source, const Point target) {
    glPointSize(1);

    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target) {}
};

#endif // __GRADIENT_BRUSH_H__