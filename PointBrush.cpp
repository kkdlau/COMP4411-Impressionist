//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "pointbrush.h"
#include "gl_helper.hpp"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

PointBrush::PointBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void PointBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();

  glPointSize((float)size);

  BrushMove(source, target);
}

void PointBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("PointBrush::BrushMove  document is NULL\n");
    return;
  }

  gl_draw(GL_POINT, [&] {
    debugger("%s\n", target.toString());

    SetColor(source);
    gl_set_point(target);
  });

  pDoc->force_update_canvas();
}

void PointBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
