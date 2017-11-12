#pragma once
#include <bits/stdc++.h>
#define FOR(i, a, b) for(int (i) = (a); (i) <= (b); ++(i))
#define rep(i, n) FOR(i, 0, n - 1)
#define rep1(i, n) FOR(i, 1, n)
#define rrep(i, n) for(int (i) = (n) - 1; (i) >= 0; --(i))
#define all(a) (a).begin(),(a).end()
constexpr int BOARD_SIZE = 8;
constexpr int MAX_TURNS = BOARD_SIZE * BOARD_SIZE - 4;
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

  bool operator== (const Point& rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  inline bool operator!= (const Point& rhs) const {
    return !(*this == rhs);
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

  ColorStorage& operator+= (const ColorStorage& clrst){
    for(int i = 0; i < 3; i++){
      data[i] += clrst.data[i];
    }
    return *this;
  }
};

using vi = vector<int>;
using vu = vector<unsigned>;
using vC = vector<Color>;
using vP = vector<Point>;
using vD = vector<Disc>;
using vvi = vector<vi>;
using vvu = vector<vu>;
using vvC = vector<vC>;
using vvP = vector<vP>;
using vvD = vector<vD>;
using vvvi = vector<vvi>;
using vvvu = vector<vvu>;
