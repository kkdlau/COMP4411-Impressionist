//
// impressionistDoc.h
//
// header file for Doc
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "Image.hpp"
#include "bitmap.h"
#include "impressionist.h"
#include <vector>

using namespace std;

class ImpressionistUI;

enum ApplicationMode { IMAGE, VIDEO };

class ImpressionistDoc {
public:
  ImpressionistDoc();

  void setUI(ImpressionistUI *ui); // Assign the UI to use

  int loadImage(char *iname); // called by the UI to load image
  int saveImage(char *iname); // called by the UI to save image

  int loadAlphaImage(char *iname);
  // TODO: implement load video and save video feature
  int loadVideo(char *iname);
  int saveVideo(char *iname);

  void toggleOriginalView();

  int clear_canvas();          // called by the UI to clear the drawing canvas
  void setBrushType(int type); // called by the UI to set the brushType
  int getSize();               // get the brush size
  void setSize(int size);      // set the brush size
  int getWidth();
  float getRad();
  float getAlpha();
  char *getImageName(); // get the current image name
  int getColorBlending();
  vector<double> getUserColor();
  void force_update_canvas();
  bool outOfRegion(const Point &p) const;
  Point clip(const Point &p);
  int getSpacing();
  int getAutoPaintRandomize();
  int getRowNum();
  int getColNum();
  bool getNormalize();
  void generate_mosaic(const char *);

  // Attributes
public:
  ApplicationMode app_mode = IMAGE;
  Image another_image;
  Image edge_image;
  Image alpha_image;

  // Dimensions of original window.
  int m_nWidth, m_nHeight;
  // Dimensions of the paint window.
  int m_nPaintWidth, m_nPaintHeight;
  // Bitmaps for original image and painting.
  unsigned char *m_ucBitmap;
  unsigned char *m_ucPainting;

  // The current active brush.
  ImpBrush *m_pCurrentBrush;
  // Size of the brush.
  int m_nSize;

  ImpressionistUI *m_pUI;

  // Operations
public:
  void swap_content();
  void undo_painting();
  void dissolve_image();
  void auto_paint();
  void multires_paint();

private:
  char m_imageName[256];
};

extern void MessageBox(char *message);
#endif
