#if !defined(__CURVED_BRUSH__)
#define __CURVED_BRUSH__
#include "ImpBrush.h"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

class CurvedBrush : public ImpBrush {
public:
    int brush_radius = 10; 
    int brush_width = 5;
    const int min_factor = 3;
    const int max_factor = 10; // max stroke length is max_factor * radius;
    const int color_threshold = 100;
    float fc = 1;
    RGBA stroke_color;
    CurvedBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL)
        : ImpBrush(pDoc, name) {}

    void BrushBegin(const Point source, const Point target, int rad, GLubyte* color) {
        ImpressionistDoc* pDoc = GetDocument();
        // width of brush
        //brush_width = pDoc->getWidth();
        glPointSize(brush_width);
        glLineWidth(brush_width);

        // brush radius - distance btw consecutive ctrl points 
        brush_radius = (rad > 0) ? rad : pDoc->getSize();
        fc = pDoc->m_pUI->getCurvature();
        BrushMove(source, target, color);
    }
    void BrushMove(const Point source, const Point target, GLubyte* color = nullptr, bool randomize = false) {
        ImpressionistDoc* pDoc = GetDocument();
        // check if target is within line 
        if (source.x <= 0 || source.x >= pDoc->m_nPaintWidth || source.y <= 0 || source.y >= pDoc->m_nPaintHeight) {
            return;
        }
        gl_draw_shape(GL_LINE_LOOP, [&] {
            // get the color 
            if (color) {
                stroke_color = make_tuple(color[0], color[1], color[2], color[3]);
                glColor4ubv(color);
            }
            else {
                SetColor(source);
                Point valid_src = pDoc->clip(source);
                stroke_color = pDoc->m_pUI->m_origView->original_img(valid_src.y, valid_src.x);
            }
            // get the control points  
            std::vector<Point> K;
            Point cur = target; // the (x, y) to calculate next 
            float last_dx = 0; float last_dy = 0;

            for (int i = 1; i <= max_factor; ++i) {
                //printf("\nControl point %d is %d %d\n", i, cur.x, cur.y);
                if (cur.x <= 0 || cur.x >= pDoc->m_nPaintWidth || cur.y <= 0 || cur.y >= pDoc->m_nPaintHeight) {
                    return;
                }
                // check color difference
                if (!ColorDiff(cur) && i > min_factor) {
                    //printf("break because of color diff\n");
                    break;
                }
                // check vanishing gradient
                auto gradient = ImageUtils::sobel(pDoc->m_pUI->m_origView->original_img, cur.y, cur.x);
                if (get<2>(gradient) == 0) {// gradient magnitude 
                    //printf("At i=%d, gradient is zero\n", i);
                    break;
                }
                float gx = get<0>(gradient);
                float gy = get<1>(gradient);
                Normalize(gx, gy);
                //printf("Normalized gradient is %f %f\n", gx, gy);
                float dx = -gy;
                float dy = gx;
                // if necessary, reverse direction
                if (last_dx * dx + last_dy * dy < 0) {
                    dx = -dx; dy = -dy;
                }
                dx = fc * dx + (1 - fc) * last_dx;
                dy = fc * dy + (1 - fc) * last_dy;
                Normalize(dx, dy);

                if (cur.x <= 0 || cur.x >= pDoc->m_nPaintWidth || cur.y <= 0 || cur.y >= pDoc->m_nPaintHeight) {
                    return;
                }
                K.push_back(cur);
                //glVertex3f(cur.x, cur.y, 0.0f);
                //gl_set_point(cur);
                cur.x += brush_radius * dx;
                cur.y += brush_radius + dy;
                last_dx = dx; last_dy = dy;
            }

            // draw with the control points 
            for (int i = 0; i < K.size(); i++) {
                Point p1 = K[i];
                DrawCircle(p1, brush_width / 2);

                if (i + 1 < K.size()) {
                    Point p2 = K[i + 1];
                    float angle = atan2(p2.y - p1.y, p2.x - p1.x);
                    Point interm(p1.x + cos(angle) * brush_radius / 2, p1.y + sin(angle) * brush_radius / 2);
                    DrawCircle(interm, brush_width / 2);
                }
            }
            });
        pDoc->force_update_canvas();
    }

    void DrawCircle(Point p, int radius) {
        for (double i = 0; i <= 2 * M_PI; i += 0.1) {
            double dx = cos(i) * radius;
            double dy = sin(i) * radius;

            gl_set_point(p.x + dx, p.y + dy);
        }
    }
    // returns true if color diff btw current and stroke_color does not exceed threshold
    bool ColorDiff(Point current) { 
        auto pixel = pDoc->m_pUI->m_origView->original_img(current.y, current.x);
        //printf("Color difference is %f", ImageUtils::colorDifference(pixel, stroke_color));
        return (ImageUtils::colorDifference(pixel, stroke_color) < color_threshold);
    }
    // get the normalized vector
    void Normalize(float& x, float& y) { 
        float magnitude = sqrt(pow(x, 2) + pow(y, 2));
        if (magnitude == 0) return;
        x = x / magnitude;
        y = y / magnitude;
    }

    void BrushEnd(const Point source, const Point target) {}
    void RandomizeAttributes() {}
    void select() {
        ImpressionistDoc* pDoc = GetDocument();
        pDoc->m_pUI->m_BrushWidthSlider->activate();
        pDoc->m_pUI->m_BrushAngleSlider->deactivate();
        pDoc->m_pUI->m_StrokeDirection->deactivate();
        pDoc->m_pUI->m_BrushSizeSlider->activate();
        pDoc->m_pUI->m_BrushAlphaSlider->activate();
        pDoc->m_pUI->m_BrushBlurSlider->deactivate();
        pDoc->m_pUI->m_ColorBlending->activate();
        pDoc->m_pUI->m_MultiResPaint->activate();
        pDoc->m_pUI->m_BrushCurvatureSlider->activate();
    }
};

#endif // __CURVED_BRUSH__
