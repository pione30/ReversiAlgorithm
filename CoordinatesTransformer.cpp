#include "Board.h"

class CoordinatesTransformer{
public:
  CoordinatesTransformer(const Point& first){
    Rotate = 0;
    Mirror = false;

    if(first == Point("d3")){
      Rotate = 1;
      Mirror = true;
    }
    else if(first == Point("c4")){
      Rotate = 2;
    }
    else if(first == Point("e6")){
      Rotate = -1;
      Mirror = true;
    }
  }

  Point normalize(const Point& p) const {
    Point newp = rotatePoint(p, Rotate);
    if(Mirror) newp = mirrorPoint(newp);

    return newp;
  }

  Point denormalize(const Point& p) const {
    Point newp(p.x, p.y);
    if(Mirror) newp = mirrorPoint(newp);

    newp = rotatePoint(newp, -Rotate);

    return newp;
  }

private:
  int Rotate;
  bool Mirror;

  Point rotatePoint(const Point& oldp, int rotate) const {
    rotate = (rotate % 4 + 4) % 4;

    Point newp;
    switch(rotate){
    case 1:
      newp.x = oldp.y;
      newp.y = BOARD_SIZE - oldp.x + 1;
      break;
    case 2:
      newp.x = BOARD_SIZE - oldp.x + 1;
      newp.y = BOARD_SIZE - oldp.y + 1;
      break;
    case 3:
      newp.x = BOARD_SIZE - oldp.y + 1;
      newp.y = oldp.x;
      break;
    default: // 0
      newp.x = oldp.x;
      newp.y = oldp.y;
      break;
    }

    return newp;
  }

  Point mirrorPoint(const Point& oldp) const {
    Point newp;
    newp.x = BOARD_SIZE - oldp.x + 1;
    newp.y = oldp.y;

    return newp;
  }

};
