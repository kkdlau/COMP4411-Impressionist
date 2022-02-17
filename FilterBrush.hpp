#if !defined(__FILTER_BRUSH_H__)
#define __FILTER_BRUSH_H__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

class FilterBrush : public ImpBrush {
    int filter_dim = 0;
public:
    FilterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}
    void BrushBegin(const Point source, const Point target) {
        ImpressionistDoc* pDoc = GetDocument();
        ImpressionistUI* dlg = pDoc->m_pUI;
        // get the blurring value 
        filter_dim = dlg->getBlurValue();
        filter_dim = (filter_dim % 2) ? filter_dim : filter_dim + 1;
        const float size = pDoc->getSize();
        glPointSize(size);
        BrushMove(source, target);
    }
    void BrushMove(const Point source, const Point target, bool randomize=false) {
        ImpressionistDoc* pDoc = GetDocument();
        if (pDoc == NULL) {
            printf("PointBrush::BrushMove document is NULL\n");
            return;
        }
        if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 || source.y >= pDoc->m_nPaintHeight) {
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
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->deactivate();
        pDoc->m_pUI->m_BrushAngleSlider->deactivate();
        pDoc->m_pUI->m_StrokeDirection->deactivate();
        pDoc->m_pUI->m_BrushSizeSlider->activate();
        pDoc->m_pUI->m_BrushAlphaSlider->activate();
        pDoc->m_pUI->m_BrushBlurSlider->activate();
        pDoc->m_pUI->m_ColorBlending->activate();
    }
    
    void RandomizeAttributes() {
        glPointSize(10);
    }
    void filterMean(const Point source, std::vector<int>& color) {
        ImpressionistDoc* pDoc = GetDocument();
        if (filter_dim < 1) {
            auto pixel = pDoc->m_pUI->m_origView->original_img(source.y, source.x);
            color[0] = get<0>(pixel);
            color[1] = get<1>(pixel);
            color[2] = get<2>(pixel);
            return;
        }
        short offset = (filter_dim - 1) / 2;
        
        int r = 0, g = 0, b = 0;
        for (int i = 0; i < filter_dim; i++) {
            for (int j = 0; j < filter_dim; j++) {
                int x = source.x - i + offset;
                int y = source.y - i + offset;
                if (x < 0) x = 0;
                else if (x >= pDoc->m_nPaintWidth) x = pDoc->m_nPaintWidth - 1;
                if (y < 0) y = 0;
                else if (y >= pDoc->m_nPaintHeight) y = pDoc->m_nPaintHeight - 1;
                auto pixel = pDoc->m_pUI->m_origView->original_img(y, x);
                r += get<0>(pixel);
                g += get<1>(pixel);
                b += get<2>(pixel);
            }
        }
        int total_pixels = filter_dim * filter_dim;
        color[0] = r / total_pixels;
        color[1] = g / total_pixels;
        color[2] = b / total_pixels;
    }
    void SetColor(const Point source) {
        ImpressionistDoc* pDoc = GetDocument();
        GLubyte color[4];
        std::vector<int> int_color{ 0, 0, 0 };
        filterMean(source, int_color); // implement blurring 
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
