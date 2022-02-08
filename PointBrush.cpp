//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "pointbrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

extern float frand();

PointBrush::PointBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void PointBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  const float size = pDoc->getSize();

  debugger("%f", size);
  glPointSize(size);

  BrushMove(source, target);
}

void PointBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("PointBrush::BrushMove  document is NULL\n");
    return;
  }

  gl_draw(GL_POINTS, [&] {
    debugger("%d, %d\n", target.x, target.y);
    SetColor(source);
    gl_set_point(target);
  });

  pDoc->force_update_canvas();
}

void PointBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
