#if !defined(__ALPHA_MAPPED_BRUSH_H__)
#define __ALPHA_MAPPED_BRUSH_H__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;
// This filter brush is for custom filters
class AlphaMappedBrush : public ImpBrush {
    bool start = true; // to get a new color from image
public:
    AlphaMappedBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}
    void BrushBegin(const Point source, const Point target) {
        // check if alpha image loaded
        ImpressionistDoc* pDoc = GetDocument();

        if (!pDoc->alpha_image.contain_content())
            printf("Please load alpha image...\n");

        //const int size = pDoc->getSize();
        glPointSize(1);
        start = true;
        BrushMove(source, target);
    }
    void BrushMove(const Point source, const Point target, bool randomize = false) {
        ImpressionistDoc* pDoc = GetDocument();
        if (pDoc == NULL) {
            printf("AlphaMappedBrush::BrushMove document is NULL\n");
            return;
        }
        if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 || source.y >= pDoc->m_nPaintHeight) {
            printf("Go back in\n");
            return;
        }

        gl_draw_shape(GL_POINTS, [&] {
            GLubyte color[4];
            for (int i = 0; i < pDoc->alpha_image.width; i++) {
                for (int j = 0; j < pDoc->alpha_image.height; j++) {
                    Point cur(i, j);
                    AlphaMappedBrush::SetColor(cur, source, color);
                    gl_set_point(cur);
                }
            }
        });

        pDoc->force_update_canvas();
    }

    void BrushEnd(const Point source, const Point target) {}

    void SetColor(const Point cur, const Point source, GLubyte* color) {
        ImpressionistDoc* pDoc = GetDocument();
        if (start) {
            Point valid_source = pDoc->clip(source);
            auto pixel = pDoc->m_pUI->m_origView->original_img(valid_source.y, valid_source.x);
            color[0] = get<0>(pixel);
            color[1] = get<1>(pixel);
            color[2] = get<2>(pixel);

            int colorBlending = pDoc->getColorBlending();

            if (colorBlending == 1) {
                vector<double> rgb = pDoc->getUserColor();
                color[0] *= rgb[0];
                color[1] *= rgb[1];
                color[2] *= rgb[2];
            }
            start = false;
        }
        Point valid_cur = pDoc->clip(cur);
        color[3] = getAlpha(valid_cur);
        glColor4ubv(color);
    }
    GLubyte getAlpha(const Point source) {
        auto pixel = pDoc->alpha_image(source.y, source.x);
        GLubyte alpha = get<0>(pixel) * 0.3 + get<1>(pixel) * 0.6 + get<2>(pixel) * 0.1;
        return alpha;
    }
    void select() {
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->deactivate();
        pDoc->m_pUI->m_BrushAngleSlider->deactivate();
        pDoc->m_pUI->m_StrokeDirection->deactivate();
        pDoc->m_pUI->m_BrushSizeSlider->activate();
        pDoc->m_pUI->m_BrushAlphaSlider->deactivate();
        pDoc->m_pUI->m_BrushBlurSlider->deactivate();
        pDoc->m_pUI->m_ColorBlending->activate();
    }

    void RandomizeAttributes() {

    }
};

#endif // __LINE_BRUSH_H__
