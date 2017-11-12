#pragma once
#include "Board.h"

class CoordinatesTransformer{
public:
  CoordinatesTransformer(const Point& first);

  Point normalize(const Point& p) const;
  Point denormalize(const Point& p) const;

private:
  int Rotate;
  bool Mirror;

  Point rotatePoint(const Point& oldp, int rotate) const;
  Point mirrorPoint(const Point& oldp) const;

};
