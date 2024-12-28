#ifndef SQUARESRENDERER_H
#define SQUARESRENDERER_H

#include "LinesRenderer.h"

struct Point {
  float x, y;


  Point operator+(const Point& other) const {
    Point result{};
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
  }
  Point operator-(const Point& other) const {
    Point result{};
    result.x = x - other.x;
    result.y = y - other.y;
    return result;
  }


  Point operator/(float num) const {
    Point result{};
    result.x = x / num;
    result.y = y / num;
    return result;
  }

};

struct Square {
  union {
    struct {
      Point p1;
      Point p2;
      Point p3;
      Point p4;
    };
    Point points[4];
  };
};

struct Triangle {
  union {
    struct {
      Point p1;
      Point p2;
      Point p3;
    };
    Point points[3];
  };
};

class SquaresRenderer {
  public:
  SquaresRenderer();
  void DrawSquare(const Point &p1, const Point &p2);
  void DrawSquare(const Square& sq);
  void DrawTriangle(const Triangle& sq);

  protected:
  LinesRenderer lines_renderer_;
};



#endif //SQUARESRENDERER_H
