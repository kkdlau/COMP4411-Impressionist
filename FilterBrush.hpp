#if !defined(__FILTER_BRUSH_H__)
#define __FILTER_BRUSH_H__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;
// This filter brush is for blurring only
class FilterBrush : public ImpBrush {
  int filter_dim = 0;

public:
  FilterBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
      : ImpBrush(pDoc, name) {}
  void BrushBegin(const Point source, const Point target, int rad,
                  GLubyte *color) {
    ImpressionistDoc *pDoc = GetDocument();
    ImpressionistUI *dlg = pDoc->m_pUI;
    // get the blurring value
    filter_dim = dlg->getBlurValue();
    filter_dim = (filter_dim % 2) ? filter_dim : filter_dim + 1;
    int size = (rad > 0) ? rad : pDoc->getSize();
    glPointSize(size);
    BrushMove(source, target);
  }
  void BrushMove(const Point source, const Point target,
                 GLubyte *color = nullptr, bool randomize = false) {
    ImpressionistDoc *pDoc = GetDocument();
    if (pDoc == NULL) {
      printf("PointBrush::BrushMove document is NULL\n");
      return;
    }
    auto &img = pDoc->m_pUI->m_paintView->cur;
    if (source.x <= 0 || source.x >= img.width || source.y <= 0 ||
        source.y >= img.height) {
      printf("Go back in\n"); // TODO - Remove
      return;
    }
    if (randomize && frand() >= 0.75)
      RandomizeAttributes();

    gl_draw_shape(GL_POINTS, [&] {
      FilterBrush::SetColor(source);
      gl_set_point(target);
    });

    pDoc->force_update_canvas();
  }

  void BrushEnd(const Point source, const Point target) {}

  void select() {
    ImpressionistDoc *pDoc = GetDocument();
    pDoc->m_pUI->m_BrushWidthSlider->deactivate();
    pDoc->m_pUI->m_BrushAngleSlider->deactivate();
    pDoc->m_pUI->m_StrokeDirection->deactivate();
    pDoc->m_pUI->m_BrushSizeSlider->activate();
    pDoc->m_pUI->m_BrushAlphaSlider->activate();
    pDoc->m_pUI->m_BrushBlurSlider->activate();
    pDoc->m_pUI->m_ColorBlending->activate();
    pDoc->m_pUI->m_MultiResPaint->deactivate();
    pDoc->m_pUI->m_BrushCurvatureSlider->deactivate();
  }

  void RandomizeAttributes() { glPointSize(irand(10)); }
  void SetColor(const Point source) {
    ImpressionistDoc *pDoc = GetDocument();
    GLubyte color[4];
    std::vector<int> int_color{0, 0, 0};
    ImageUtils::filterMean(pDoc->m_pUI->m_origView->original_img, source,
                           int_color, filter_dim); // implement blurring
    color[0] = int_color[0];
    color[1] = int_color[1];
    color[2] = int_color[2];

    int colorBlending = pDoc->getColorBlending();

    if (colorBlending == 1) {
      vector<double> rgb = pDoc->getUserColor();
      color[0] *= rgb[0];
      color[1] *= rgb[1];
      color[2] *= rgb[2];
    }

    const float alpha = pDoc->getAlpha();
    color[3] = (GLubyte)(alpha * 255.0f);

    glColor4ubv(color);
  }
};

#endif // __LINE_BRUSH_H__
