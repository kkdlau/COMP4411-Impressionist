//
// PointBrush.h
//
// The header file for Point Brush.
//

#ifndef POINTBRUSH_H
#define POINTBRUSH_H

#include "ImpBrush.h"
#include "impressionistDoc.h"

class PointBrush : public ImpBrush {
public:
  PointBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target, int rad, GLubyte* color);
  void BrushMove(const Point source, const Point target, GLubyte* color = nullptr, bool randomize = false);
  void BrushEnd(const Point source, const Point target);
  void RandomizeAttributes();
  void select();

  char *BrushName(void);
};

#endif