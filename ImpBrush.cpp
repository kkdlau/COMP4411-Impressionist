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

  GLubyte color[3];

  memcpy(color, pDoc->GetOriginalPixel(source), 3);

  glColor3ubv(color);
}

void ImpBrush::set_brush(int index, ImpBrush *b) {
  ImpBrush::c_pBrushes[index] = b;
}
