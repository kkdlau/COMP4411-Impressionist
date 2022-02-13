//
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the
// brush. It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "CircleBrush.hpp"
#include "Image.hpp"
#include "LineBrush.hpp"
#include "PointBrush.h"
#include "ScatteredCircleBrush.hpp"
#include "ScatteredLineBrush.hpp"
#include "ScatteredPointBrush.hpp"
#include "gl_helper.h"

#define DESTROY(p)                                                             \
  {                                                                            \
    if ((p) != NULL) {                                                         \
      delete[] p;                                                              \
      p = NULL;                                                                \
    }                                                                          \
  }

ImpressionistDoc::ImpressionistDoc() {
  // Set NULL image name as init.

  m_imageName[0] = '\0';

  m_nWidth = -1;
  m_ucBitmap = NULL;
  m_ucPainting = NULL;

  // create one instance of each brush
  ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
  ImpBrush::c_pBrushes = new ImpBrush *[ImpBrush::c_nBrushCount];

  ImpBrush::set_brush(BRUSH_POINTS, new PointBrush(this, "Points"));

  // Note: You should implement these 5 brushes.  They are set the same
  // (PointBrush) for now
  ImpBrush::set_brush(BRUSH_LINES, new LineBrush(this, "Lines"));
  ImpBrush::set_brush(BRUSH_CIRCLES, new CircleBrush(this, "Circles"));
  ImpBrush::set_brush(BRUSH_SCATTERED_POINTS,
                      new ScatteredPointBrush(this, "Scattered Points"));
  ImpBrush::set_brush(BRUSH_SCATTERED_LINES,
                      new ScatteredLineBrush(this, "Scattered Lines"));
  ImpBrush::set_brush(BRUSH_SCATTERED_CIRCLES,
                      new ScatteredCircleBrush(this, "Scattered Circles"));
  // make one of the brushes current
  m_pCurrentBrush = ImpBrush::c_pBrushes[0];
  GLHelper::set_doc(this);
}

//---------------------------------------------------------
// Set the current UI
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI *ui) {
  m_pUI = ui;
  GLHelper::set_ui(m_pUI);
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char *ImpressionistDoc::getImageName() { return m_imageName; }

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type) {
  m_pCurrentBrush = ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize() { return m_pUI->getSize(); }

int ImpressionistDoc::getWidth() { return m_pUI->getWidth(); }

float ImpressionistDoc::getRad() { return m_pUI->getAngle() / 180.0 * M_PI; }

float ImpressionistDoc::getAlpha() { return m_pUI->getAlpha(); }
//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) {
  // try to open the image to read
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }

  // reflect the fact of loading the new image
  m_nWidth = width;
  m_nPaintWidth = width;
  m_nHeight = height;
  m_nPaintHeight = height;

  // release old storage
  if (m_ucBitmap)
    delete[] m_ucBitmap;
  if (m_ucPainting)
    delete[] m_ucPainting;

  m_ucBitmap = data;
  m_pUI->m_origView->original_img.set(data, width, height);
  m_pUI->m_origView->img.set(data, width, height);

  // allocate space for draw view
  m_ucPainting = new unsigned char[width * height * 3];
  memset(m_ucPainting, 0, width * height * 3);

  m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
                              m_pUI->m_mainWindow->y(), width * 2, height + 25);

  // display it on origView
  m_pUI->m_origView->set_current_img(m_pUI->m_origView->original_img);

  // refresh paint view as well
  m_pUI->m_paintView->resizeWindow(width, height);
  m_pUI->m_paintView->refresh();

  m_pUI->m_paintView->prev.set(m_ucPainting, width, height);
  m_pUI->m_paintView->cur.set(m_ucPainting, width, height);

  return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) {

  writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

  return 1;
}

int ImpressionistDoc::loadVideo(char *iname) { return -1; }

int ImpressionistDoc::saveVideo(char *iname) { return -1; }

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() {

  // Release old storage
  if (m_ucPainting) {
    delete[] m_ucPainting;

    // allocate space for draw view
    m_ucPainting = new unsigned char[m_nPaintWidth * m_nPaintHeight * 3];
    memset(m_ucPainting, 0, m_nPaintWidth * m_nPaintHeight * 3);

    // refresh paint view as well
    m_pUI->m_paintView->refresh();
  }

  return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte *ImpressionistDoc::GetOriginalPixel(int x, int y) {
  if (x < 0)
    x = 0;
  else if (x >= m_nWidth)
    x = m_nWidth - 1;

  if (y < 0)
    y = 0;
  else if (y >= m_nHeight)
    y = m_nHeight - 1;

  return (GLubyte *)(m_ucBitmap + 3 * (y * m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte *ImpressionistDoc::GetOriginalPixel(const Point p) {
  return GetOriginalPixel(p.x, p.y);
}

void ImpressionistDoc::toggleOriginalView() {
  // TODO： toggle original view
}

void ImpressionistDoc::force_update_canvas() {

#ifdef __APPLE__
  m_pUI->m_paintView->refresh();
  m_pUI->m_origView->refresh();
  m_pUI->m_mainWindow->redraw();
#endif
}

bool ImpressionistDoc::outOfRegion(const Point &p) const {
  return !(p.x >= 0 && p.y >= 0 && p.x <= m_nPaintWidth &&
           p.y <= m_nPaintHeight);
}

Point ImpressionistDoc::clip(const Point &p) {
  return m_pUI->m_origView->img.clip(p);
}

void ImpressionistDoc::swap_content() {
  Image left_canvas_img = m_pUI->m_origView->original_img;
  Image right_canvas_img = m_pUI->m_paintView->cur;
  m_pUI->m_paintView->set_current_img(left_canvas_img);
  m_pUI->m_origView->set_current_img(right_canvas_img);
}
void ImpressionistDoc::undo_painting() {
  PaintView &canvas = *m_pUI->m_paintView;
  canvas.set_current_img(canvas.prev);
}