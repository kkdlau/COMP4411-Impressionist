#if !defined(__PULL_BRUSH__)
#define __PULL_BRUSH__
#include "Image.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

extern float frand();

class PullBrush : public ImpBrush {
public:
  PullBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}
  Point last;

  void BrushBegin(const Point source, const Point target, int rad,
                  GLubyte *color) {
    glPointSize(1);
    last = target;

    BrushMove(source, target);
  }

  void BrushMove(const Point source, const Point target,
                 GLubyte *color = nullptr, bool randomize = false) {
    printf("pull\n");
    fflush(stdout);
    ImpressionistDoc *pDoc = GetDocument();
    if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 ||
        source.y >= pDoc->m_nPaintHeight) {
      return;
    }
    const int half = pDoc->getSize() / 2;
    const int width = pDoc->getWidth() / 2;
    float r = 0;

    Point rt = Point::zero().shift_y(-width).shift_x(half);
    Point rb = Point::zero().shift_y(width).shift_x(half);

    Point lt = Point::zero().shift_y(-width).shift_x(-half);
    Point lb = Point::zero().shift_y(width).shift_x(-half);

    const int dir = (target.x - last.x > 0) ? 10 : -10;

    gl_draw_shape(GL_POINTS, [&] {
      // for (int y = lt.y; y <= rb.y; y++) {
      //   for (int x = lt.x; x <= rb.x; x++) {
      for (int y = -20; y <= 20; y++) {
        for (int x = -20; x <= 20; x++) {
          Point p = {x, y};
          fflush(stdout);
          Point scolor_p = {x + dir, y};
          p = target + p.rotate(r);
          scolor_p = target + scolor_p.rotate(r);
          Image &cur = pDoc->m_pUI->m_origView->original_img;
          auto valid_point = cur.valid_point(p.y, p.x);
          auto src_valid_point = cur.valid_point(scolor_p.y, scolor_p.x);

          if (valid_point) {
            printf("%d, %d failed\n", p.x, p.y);
          }

          if (src_valid_point) {
            printf("target: %d, %d failed\n", scolor_p.x, scolor_p.y);
          }

          if (!cur.valid_point(p.y, p.x) ||
              !cur.valid_point(scolor_p.y, scolor_p.x))
            continue;

          SetColor(scolor_p);
          gl_set_point(p);
        }
      }
    });
    pDoc->force_update_canvas();
    last = target;
  }

  void BrushEnd(const Point source, const Point target) {
    // do nothing so far
  }

  void select() {
    ImpressionistDoc *pDoc = GetDocument();
    pDoc->m_pUI->m_BrushWidthSlider->activate();
    pDoc->m_pUI->m_BrushAngleSlider->deactivate();
    pDoc->m_pUI->m_StrokeDirection->deactivate();
    pDoc->m_pUI->m_BrushSizeSlider->activate();
    pDoc->m_pUI->m_BrushAlphaSlider->activate();
    pDoc->m_pUI->m_BrushBlurSlider->deactivate();
    pDoc->m_pUI->m_ColorBlending->activate();
    pDoc->m_pUI->m_MultiResPaint->deactivate();
  }

  void RandomizeAttributes() {
    ImpressionistDoc *pDoc = GetDocument();
    int width = pDoc->getWidth();
    glLineWidth(irand(width + 5));
  }
};

#endif // __PULL_BRUSH__
