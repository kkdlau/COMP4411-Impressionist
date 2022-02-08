#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <cmath>
#include <stdlib.h>
#include <string>
#include <typeinfo>

// Each brush type has an associated constant.
enum {
  BRUSH_POINTS = 0,
  BRUSH_LINES,
  BRUSH_CIRCLES,
  BRUSH_SCATTERED_POINTS,
  BRUSH_SCATTERED_LINES,
  BRUSH_SCATTERED_CIRCLES,
  NUM_BRUSH_TYPE // Make sure this stays at the end!
};

class ImpressionistDoc; // Pre-declaring class

class Point {
public:
  Point(){};
  Point(int xx, int yy) {
    x = xx;
    y = yy;
  };

  int x, y;

  friend Point operator+(const Point &p1, const Point &p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
  }

  friend Point operator-(const Point &p1, const Point &p2) {
    return Point(p1.x - p2.x, p1.y - p2.y);
  }

  const char *toString() const {
    return (std::to_string(x) + ", " + std::to_string(y)).c_str();
  }

  Point shift_x(int x) const { return Point(this->x + x, y); }
  Point shift_y(int y) const { return Point(x, this->y + y); }
  Point rotate(float rad) const {
    return Point((int)(cos(rad) * (x)-sin(rad) * y),
                 (int)(sin(rad) * (x)-cos(rad) * y));
  }

  static Point zero() { return Point(0, 0); }
};

#define FOR_EACH_BRUSH(v) for (short v = 0; i < ImpBrush::c_nBrushCount; v++)

class ImpBrush {
protected:
  ImpBrush(ImpressionistDoc *pDoc = NULL, const char *name = NULL);

public:
  // The implementation of your brush should realize these virtual functions
  virtual void BrushBegin(const Point source, const Point target) = 0;
  virtual void BrushMove(const Point source, const Point target) = 0;
  virtual void BrushEnd(const Point source, const Point target) = 0;

  // according to the source image and the position, determine the draw color
  void SetColor(const Point source);

  // get Doc to communicate with it
  ImpressionistDoc *GetDocument(void);

  // Return the name of the brush (not used in this version).
  const char *BrushName(void);

  static int c_nBrushCount;     // How many brushes we have,
  static ImpBrush **c_pBrushes; // and what they are.
  static void set_brush(int index, ImpBrush *b);

private:
  ImpressionistDoc *m_pDoc;

  // Brush's name (not used in this version).
  const char *m_pBrushName;
};

#endif