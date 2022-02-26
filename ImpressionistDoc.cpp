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
#include "AlphaMappedBrush.hpp"
#include "ArcBrush.hpp"
#include "CircleBrush.hpp"
#include "CurvedBrush.hpp"
#include "CustomFilterBrush.hpp"
#include "FanBrush.hpp"
#include "FilterBrush.hpp"
#include "GradientBrush.hpp"
#include "Image.hpp"
#include "LineBrush.hpp"
#include "PointBrush.h"
#include "PullBrush.hpp"
#include "ScatteredCircleBrush.hpp"
#include "ScatteredLineBrush.hpp"
#include "ScatteredPointBrush.hpp"
#include "VideoUtils.hpp"
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
  ImpBrush::set_brush(BRUSH_FANS, new FanBrush(this, "Fans"));
  ImpBrush::set_brush(BRUSH_ARC, new ArcBrush(this, "Curves"));
  ImpBrush::set_brush(BRUSH_FILTER, new FilterBrush(this, "Blurring"));
  ImpBrush::set_brush(BRUSH_GRADIENT, new GradientBrush(this, "Gradient"));
  ImpBrush::set_brush(BRUSH_CUSTOM_FILTER,
                      new CustomFilterBrush(this, "Custom Filter"));
  ImpBrush::set_brush(BRUSH_ALPHA, new AlphaMappedBrush(this, "Alpha-mapped"));
  ImpBrush::set_brush(BRUSH_PULL_AS_RUBBER, new PullBrush(this, "Rubber"));
  ImpBrush::set_brush(BRUSH_CURVED, new CurvedBrush(this, "Curved"));

  // make one of the brushes current
  m_pCurrentBrush = ImpBrush::c_pBrushes[0];
  GLHelper::set_doc(this);
}

//---------------------------------------------------------
// Set the current UI
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI *ui) { m_pUI = ui; }

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

int ImpressionistDoc::getColorBlending() { return m_pUI->getColorBlending(); }

vector<double> ImpressionistDoc::getUserColor() {
  return m_pUI->getUserColor();
}
int ImpressionistDoc::getSpacing() { return m_pUI->getSpacing(); }

int ImpressionistDoc::getAutoPaintRandomize() {
  return m_pUI->getAutoPaintRandomize();
}

int ImpressionistDoc::getRowNum() { return m_pUI->getRowNum(); }

int ImpressionistDoc::getColNum() { return m_pUI->getColNum(); }

bool ImpressionistDoc::getNormalize() { return m_pUI->getNormalize(); }
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
  // if (m_ucBitmap)
  //  delete[] m_ucBitmap;
  // if (m_ucPainting)
  // delete[] m_ucPainting;

  // clear the original image
  if (another_image.contain_content()) {
    another_image.clear();
    m_pUI->set_use_another_gradient(false);
    m_pUI->m_another_gradient_checkbox->clear();
  }

  if (edge_image.contain_content()) {
    edge_image.clear();
    m_pUI->set_use_another_gradient(false);
    m_pUI->m_edge_clipping_checkbox->clear();
  }

  if (alpha_image.contain_content()) {
    alpha_image.clear();
  }

  app_mode = IMAGE;

  OriginalView &orig_view = *m_pUI->m_origView;
  PaintView &paint_view = *m_pUI->m_paintView;

  m_ucBitmap = data;
  orig_view.original_img.set(data, width, height);
  orig_view.img.set(data, width, height);

  // allocate space for draw view
  m_ucPainting = new unsigned char[width * height * 3];
  memset(m_ucPainting, 0, width * height * 3);

  m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
                              m_pUI->m_mainWindow->y(), width * 2, height + 25);

  // display it on origView
  orig_view.set_current_img(orig_view.original_img);

  paint_view.prev.set(m_ucPainting, width, height);
  paint_view.cur.set(m_ucPainting, width, height);

  paint_view.set_current_img(paint_view.cur);

  return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) {
  Image &cur = m_pUI->m_paintView->cur;
  unsigned char *data = cur.exportable_fmt();
  writeBMP(iname, cur.width, cur.height, data);

  return 1;
}

int ImpressionistDoc::loadAlphaImage(char *iname) {
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }
  if (alpha_image.contain_content())
    alpha_image.clear();
  alpha_image.set(data, width, height);
  printf("successfully loaded alpha image %d %d\n", alpha_image.width,
         alpha_image.height);
  return 1;
}

int ImpressionistDoc::loadVideo(char *iname) {
  // implemen the logic
  VideoUtils::open_video(iname);
  Image first_frame = VideoUtils::get_frame(0);

  app_mode = VIDEO;

  int width = first_frame.width;
  int height = first_frame.height;

  m_nWidth = width;
  m_nPaintWidth = width;
  m_nHeight = height;
  m_nPaintHeight = height;

  // clear the original image
  if (another_image.contain_content()) {
    another_image.clear();
    m_pUI->set_use_another_gradient(false);
    m_pUI->m_another_gradient_checkbox->clear();
  }

  if (edge_image.contain_content()) {
    edge_image.clear();
    m_pUI->set_use_another_gradient(false);
    m_pUI->m_edge_clipping_checkbox->clear();
  }

  if (alpha_image.contain_content()) {
    alpha_image.clear();
  }
  OriginalView &orig_view = *m_pUI->m_origView;
  PaintView &paint_view = *m_pUI->m_paintView;

  // resize the window
  m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
                              m_pUI->m_mainWindow->y(), width * 2, height + 25);

  // generate a black image
  m_ucPainting = new unsigned char[width * height * 3];
  memset(m_ucPainting, 0, width * height * 3);

  // set the previous painting and current painting to the black image
  paint_view.prev.set(m_ucPainting, width, height);
  paint_view.cur.set(m_ucPainting, width, height);

  paint_view.set_current_img(paint_view.cur);
  orig_view.set_current_img(first_frame);

  return 0;
}

int ImpressionistDoc::saveVideo(char *iname) { return -1; }

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clear_canvas() {

  // Release old storage
  if (m_ucPainting) {
    delete[] m_ucPainting;

    // allocate space for draw view
    m_ucPainting = new unsigned char[m_nPaintWidth * m_nPaintHeight * 3];
    memset(m_ucPainting, 0, m_nPaintWidth * m_nPaintHeight * 3);
    Image img{m_ucPainting, m_nPaintWidth, m_nPaintHeight};
    m_pUI->m_paintView->set_current_img(img);
  }

  return 0;
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
void ImpressionistDoc::auto_paint() {
  PaintView &canvas = *m_pUI->m_paintView;
  canvas.auto_paint_flag = true;
  canvas.refresh();
  // canvas.auto_paint();
}
void ImpressionistDoc::multires_paint() {
  PaintView &canvas = *m_pUI->m_paintView;
  canvas.multires_paint_flag = true;
  canvas.refresh();
  // canvas.multires_paint();
}

void ImpressionistDoc::generate_mosaic(const char *d) {
  Image &original_img = m_pUI->m_origView->original_img;
  ImageUtils::mosaics(original_img, d);
}