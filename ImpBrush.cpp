//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "ImpBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cstring>

// Static class member initializations
int ImpBrush::c_nBrushCount = 0;
ImpBrush **ImpBrush::c_pBrushes = nullptr;
const short ImpBrush::mean_filter[3][3] = {
    {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
};
const short ImpBrush::gaussian_filter[3][3] = {
    {1, 2, 1}, {2, 4, 2}, {1, 2, 1}
};

ImpBrush::ImpBrush(ImpressionistDoc *pDoc, const char *name)
    : m_pDoc(pDoc), m_pBrushName(name) {
  // ImpBrush **brush_list = new ImpBrush *[c_nBrushCount + 1];
  // FOR_EACH_BRUSH(i) { brush_list[i] = c_pBrushes[i]; }
  // brush_list[c_nBrushCount++] = this;
  // delete[] c_pBrushes;
  // c_pBrushes = brush_list;
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc *ImpBrush::GetDocument(void) { return m_pDoc; }

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
const char *ImpBrush::BrushName(void) { return m_pBrushName; }

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample
// the color from
//----------------------------------------------------
void ImpBrush::SetColor(const Point source) {
  ImpressionistDoc *pDoc = GetDocument();
  GLubyte color[4];
  BrushFilter brush_filter = pDoc->m_pUI->get_filter();
  std::vector<int> int_color{ 0, 0, 0 };
  auto pixel = pDoc->m_pUI->m_origView->original_img(source.y, source.x);

  switch (brush_filter) {
  case FILTER_NONE:
      color[0] = get<0>(pixel);
      color[1] = get<1>(pixel);
      color[2] = get<2>(pixel);
      break;
  
  case FILTER_BLUR:
      filter(gaussian_filter, 16, 3, source, int_color);
      color[0] = int_color[0];
      color[1] = int_color[1];
      color[2] = int_color[2];
      break;
  }
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
void ImpBrush::filter(const short filter[][3], const int divisor, const int dim, const Point source, std::vector<int>& color) {
    // two for-loops for convolution
    // change the contents of color as necessary
    // for out-of-bounds, use the value of the border 
    short offset = (dim - 1) / 2;
    ImpressionistDoc* pDoc = GetDocument();
    int r = 0, g = 0, b = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int x = source.x - i + offset;
            int y = source.y - i + offset;
            if (x < 0) x = 0;
            else if (x >= pDoc->m_nPaintWidth) x = pDoc->m_nPaintWidth - 1;
            if (y < 0) y = 0;
            else if (y >= pDoc->m_nPaintHeight) y = pDoc->m_nPaintHeight - 1;

            auto pixel = pDoc->m_pUI->m_origView->original_img(y, x);
            r += get<0>(pixel) * filter[i][j];
            g += get<1>(pixel) * filter[i][j];
            b += get<2>(pixel) * filter[i][j];
        }
    }
    color[0] = r / divisor;
    color[1] = g / divisor;
    color[2] = b / divisor;
}

void ImpBrush::set_brush(int index, ImpBrush *b) {
  ImpBrush::c_pBrushes[index] = b;
}
