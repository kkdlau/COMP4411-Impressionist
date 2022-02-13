#if !defined(__CURVE_BRUSH__)
#define __CURVE_BRUSH__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class CurveBrush : public ImpBrush {
    Point last;
public:
    CurveBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}

    void BrushBegin(const Point source, const Point target) {
        ImpressionistDoc* pDoc = GetDocument();
        last = target;
        const int width = pDoc->getWidth();
        glPointSize(width);
        BrushMove(source, target);
    }

    void BrushMove(const Point source, const Point target) {
        ImpressionistDoc* pDoc = GetDocument();

        const double radius = 2 * pDoc->getSize();
        const int width = pDoc->getWidth();
        float r = pDoc->getRad();

        switch (pDoc->m_pUI->get_direction()) {
        case GRADIENT_DIRECTION: {
            auto result = ImageUtils::sobel(pDoc->m_pUI->m_origView->original_img,
                target.y, target.x);
            r = get<2>(result);
        } break;
        case BRUSH_DIRECTION: {
            r = target / last;
        } break;
        }

        if (isnan(r)) return;

        gl_draw_shape(GL_POINTS, [&] {
            SetColor(source);
            for (double i = M_PI / 4; i <= 3 * M_PI / 4; i += 0.01) {
                double angle = i + r;
                double dx = cos(angle) * radius;
                double dy = sin(angle) * radius;
                gl_set_point(target.x + dx, target.y + dy);
            }
            });

        pDoc->force_update_canvas();
    }
    void BrushEnd(const Point source, const Point target) {}
    void select() {
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->activate();
        pDoc->m_pUI->m_BrushAngleSlider->activate();
        pDoc->m_pUI->m_StrokeDirection->activate();
    }
};

#endif // __CIRCLE_BRUSH__