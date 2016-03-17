#pragma once
#include <bits/stdc++.h>
using namespace std;

enum Color{
  EMPTY = 0,
  WHITE = -1,
  BLACK = 1,
  WALL = 2
};

struct Point{
  int x;
  int y;
  Point(){ x = 0; y = 0; }
  Point(int _x, int _y) : x(_x), y(_y){}

  Point(string coordstr) throw(std::invalid_argument){
    if(coordstr.length() < 2)
      throw std::invalid_argument(
          "The argument must be Reversi style coordinates!" );

    x = coordstr[0] - 'a' + 1;
    y = coordstr[1] - '1' + 1;
  }

  operator string() const {
    ostringstream oss;
    oss << static_cast<char>('a' + x - 1)
        << static_cast<char>('1' + y - 1);

    return oss.str();
  }

};

struct Disc : public Point{
  Color color;
  Disc(){ color = Color::EMPTY; }
  Disc(int _x, int _y, Color _color) : Point(_x, _y), color(_color){}
};

template<typename T> class ColorStorage{
private:
  array<T, 3> data;
public:
  T& operator[](Color color){
    return data[color + 1];
  }
  const T& operator[](Color color) const {
    return data[color + 1];
  }
}; 
