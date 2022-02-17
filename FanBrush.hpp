#if !defined(__FAN_BRUSH__)
#define __FAN_BRUSH__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

using namespace GLHelper;

class FanBrush : public ImpBrush {
    Point last;
    double radius;
public:
    FanBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}

    void BrushBegin(const Point source, const Point target) {
        ImpressionistDoc* pDoc = GetDocument();
        last = target;
        BrushMove(source, target);
    }

    void BrushMove(const Point source, const Point target, bool randomize=false) {
        ImpressionistDoc* pDoc = GetDocument();
        
        radius = pDoc->getSize();
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

        if (randomize && frand() >= 0.75)
            RandomizeAttributes();

        gl_draw_shape(GL_TRIANGLE_FAN, [&] {
            SetColor(source);
            gl_set_point(target.x, target.y);
            for (double i = M_PI / 6; i <= 5 * M_PI / 6; i += 0.1) {
                double angle = i + r;
                double dx = cos(angle) * radius;
                double dy = sin(angle) * radius;
                gl_set_point(target.x + dx, target.y + dy);
            }
            });

        pDoc->force_update_canvas();
    }
    void BrushEnd(const Point source, const Point target) {}

    void RandomizeAttributes() {
        radius = irand(20);
    }

    void select() {
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->deactivate();
        pDoc->m_pUI->m_BrushAngleSlider->activate();
        pDoc->m_pUI->m_StrokeDirection->activate();
        pDoc->m_pUI->m_BrushSizeSlider->activate();
        pDoc->m_pUI->m_BrushAlphaSlider->activate();
        pDoc->m_pUI->m_BrushBlurSlider->deactivate();
        pDoc->m_pUI->m_ColorBlending->activate();
    }
};

#endif // __CIRCLE_BRUSH__
