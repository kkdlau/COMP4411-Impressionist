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
#include <vector>
#include <typeinfo>

extern float frand();

// Each brush type has an associated constant.
enum {
  BRUSH_POINTS = 0,
  BRUSH_LINES,
  BRUSH_CIRCLES,
  BRUSH_SCATTERED_POINTS,
  BRUSH_SCATTERED_LINES,
  BRUSH_SCATTERED_CIRCLES,
  BRUSH_FANS,
  BRUSH_CURVES,
  BRUSH_FILTER, // brush with blurring filter
  NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum StrokeDirection {
  SLIDER_RIGHT_MOUSE,
  GRADIENT_DIRECTION,
  BRUSH_DIRECTION,
  NUM_STROKE_DIRECTION_METHODS
};

enum BrushFilter {
    FILTER_NONE = 0,
    FILTER_BLUR,
    NUM_BRUSH_FILTER
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

  /**
   * @brief Point division operation.
   *
   * This function will return the radian by calculating tan((p2.y - p1.y) /
   * (p2.x - p1.x)).
   */
  friend float operator/(const Point &p1, const Point &p2) {
    return atan2((float)(p2.y - p1.y), (float)(p2.x - p1.x));
  }

  const char *toString() const {
    return (std::to_string(this->x) + ", " + std::to_string(this->y)).c_str();
  }

  Point shift_x(int x) const { return Point(this->x + x, y); }
  Point shift_y(int y) const { return Point(x, this->y + y); }
  Point rotate(float rad) const {
    return Point((int)(cos(rad) * (x)-sin(rad) * y),
                 (int)(sin(rad) * (x)-cos(rad) * y));
  }

  static Point zero() { return Point(0, 0); }
  static Point rand(float min, float max) {
    const float span = max - min;
    return Point(frand() * span + min, frand() * span + min);
  }
};

#define FOR_EACH_BRUSH(v) for (short v = 0; i < ImpBrush::c_nBrushCount; v++)

class ImpBrush {
protected:
  ImpBrush(ImpressionistDoc *pDoc = NULL, const char *name = NULL);

public:
  // The implementation of your brush should realize these virtual functions
  virtual void BrushBegin(const Point source, const Point target) = 0;
  virtual void BrushMove(const Point source, const Point target, bool randomize=false) = 0;
  virtual void BrushEnd(const Point source, const Point target) = 0;
  virtual void RandomizeAttributes() = 0;
  // according to the source image and the position, determine the draw color
  void SetColor(const Point source);

  void filter(const short filter[][3], const int divisor, const int dim, const Point source, std::vector<int>& color);

  // TODO: implement all UI enable / disable business logic in here.
  virtual void select() {}
  //virtual void disselect() {}

  // get Doc to communicate with it
  ImpressionistDoc *GetDocument(void);

  // Return the name of the brush (not used in this version).
  const char *BrushName(void);

  static int c_nBrushCount;     // How many brushes we have,
  static ImpBrush **c_pBrushes; // and what they are.
  static void set_brush(int index, ImpBrush *b);
  static const short gaussian_filter[3][3];

private:
  ImpressionistDoc *m_pDoc;

  // Brush's name (not used in this version).
  const char *m_pBrushName;
};

#endif