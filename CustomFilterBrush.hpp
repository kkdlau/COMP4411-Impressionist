#if !defined(__CUSTOM_FILTER_BRUSH_H__)
#define __CUSTOM_FILTER_BRUSH_H__
#include "ImageUtils.hpp"
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;
// This filter brush is for custom filters
class CustomFilterBrush : public ImpBrush {
    char filter_val_str[400];
    std::vector<float> filter_vals;
    int row_num = 0;
    int col_num = 0;
    int divisor = 1;
    int sum = 0;
public:
    CustomFilterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}
    void BrushBegin(const Point source, const Point target, int rad) {
        ImpressionistDoc* pDoc = GetDocument();
        if (pDoc->m_pUI->getFilterValues(filter_val_str) > 0) {
            row_num = pDoc->getRowNum();
            col_num = pDoc->getColNum();
            parseFilterString();
            printf("size of filter: %d\n", filter_vals.size());
        }
        if (!checkValidFilter()) {
            printf("Invalid filter...\n");
            return;
        }
        divisor = (pDoc->getNormalize() && sum > 0) ? sum : 1;
        printf("The divisor is %d\n", divisor);
        int size = (rad > 0)? rad : pDoc->getSize();
        glPointSize(size);
        BrushMove(source, target);
    }
    void BrushMove(const Point source, const Point target, bool randomize = false) {
        if (!checkValidFilter()) {
            printf("Invalid filter...\n");
            return;
        }
        ImpressionistDoc* pDoc = GetDocument();
        if (pDoc == NULL) {
            printf("PointBrush::BrushMove document is NULL\n");
            return;
        }
        if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 || source.y >= pDoc->m_nPaintHeight) {
            printf("Go back in\n");
            return;
        }
        if (randomize && frand() >= 0.75)
            RandomizeAttributes();

        gl_draw_shape(GL_POINTS, [&] {
            CustomFilterBrush::SetColor(source);
            gl_set_point(target);
        });

        pDoc->force_update_canvas();
    }

    void BrushEnd(const Point source, const Point target) {}

    void parseFilterString() {
        // break according to newline
        // break according to commas
        // only put as enough for col_num and row_num into filter_vals
        // sum all such values for divisor 
        //printf("Start parseFilterString\n");
        sum = 0;
        filter_vals.clear();
        std::vector<char*> v;
        int row = 0, col = 0;
        char* chars_array = strtok(filter_val_str, "\n");
        while (chars_array && row++ < row_num) {
            v.push_back(chars_array);
            chars_array = strtok(NULL, "\n");
            //printf("Getting row %d\n", row);
        }
        for (int i = 0; i < row; ++i) {
            char* subchar_array = strtok(v[i], ",");
            while (subchar_array && col++ < col_num) {
                float val = (float)atof(subchar_array);
                filter_vals.push_back(val);
                sum += val;
            }
            while (col++ < col_num) {
                filter_vals.push_back(0);
            }
            col = 0;
            //printf("Got row %d\n", i);
        }
        while (row++ < row_num) {
            for (int i = 0; i < col_num; ++i)
                filter_vals.push_back(0);
            //printf("Putting in missing row %d\n", row);
        }
    }
    void SetColor(const Point source) {
        ImpressionistDoc* pDoc = GetDocument();
        GLubyte color[4];
        std::vector<int> int_color{ 0, 0, 0 };
        ImageUtils::filterCustom(pDoc->m_pUI->m_origView->original_img, source, int_color,
            filter_vals, row_num, col_num, divisor); 
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
    bool checkValidFilter() const {
        return (row_num > 0 && col_num > 0);
    }

    void select() {
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->deactivate();
        pDoc->m_pUI->m_BrushAngleSlider->deactivate();
        pDoc->m_pUI->m_StrokeDirection->deactivate();
        pDoc->m_pUI->m_BrushSizeSlider->activate();
        pDoc->m_pUI->m_BrushAlphaSlider->activate();
        pDoc->m_pUI->m_BrushBlurSlider->deactivate();
        pDoc->m_pUI->m_ColorBlending->activate();
        pDoc->m_pUI->m_MultiResPaint->activate();
    }

    void RandomizeAttributes() {
        glPointSize(irand(10));
    }
};

#endif // __LINE_BRUSH_H__