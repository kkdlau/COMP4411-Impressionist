#if !defined(__CURVE_BRUSH_H__)
#define __CURVE_BRUSH_H__
#include "ImageUtils.hpp"
#include "LineBrush.hpp"
#include "gl_helper.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cmath>

using namespace GLHelper;

class CurveBrush : public LineBrush {
	Point last;
public:
	CurveBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL)
		: LineBrush(pDoc, name) {}

	// inherit BrushBegin, BrushEnd, select from LineBrush

	void BrushMove(const Point source, const Point target) {
		ImpressionistDoc* pDoc = GetDocument();
		const int half = pDoc->getSize() / 2;
		const int width = pDoc->getWidth();
		/**
		// set direction of curve (line btw start & end)
		float r = pDoc->getRad();
		switch (pDoc->m_pUI->get_direction()) {
		case GRADIENT_DIRECTION: {
			auto result = ImageUtils::sobel(
				pDoc->m_pUI->m_origView->original_img, target.y, target.x
			);
			r = get<2>(result);
			break;
		}
		case BRUSH_DIRECTION: {
			r = target / last;
			break;
		}
		}
		if (isnan(r)) return;
		**/
		float r = 0; // assume no angle specified first 
		gl_draw_shape(GL_LINE_STRIP, [&] {
			SetColor(source);
			Point p0 = target + Point::zero().shift_x(-half).rotate(r);
			Point p1 = target + Point::zero().shift_y(width + 5).rotate(r);
			Point p2 = target + Point::zero().shift_x(half).rotate(r);
			printf("p0: (%d, %d)\n", p0.x, p0.y);
			printf("p1: (%d, %d)\n", p1.x, p1.y);
			printf("p2: (%d, %d)\n", p2.x, p2.y);
			const float k = 0.66;
			//(1–t)2P0 + 2(1–t)tP1 + t2P2
			for (int i = 0; i < 5; ++i) {
				double t = i / 5 * k;
				double x = pow(1 - t, 2) * p0.x + 2 * (1 - t) * t * p1.x + pow(t, 2) * p2.x;
				double y = pow(1 - t, 2) * p0.y + 2 * (1 - t) * t * p1.y + pow(t, 2) * p2.y;
				printf("Current point is (%f, %f)\n", x, y);
				gl_set_point(x, y);
			}
		});

		pDoc->force_update_canvas();

		last = target;
	}
};

#endif // __CURVE_BRUSH_H__
