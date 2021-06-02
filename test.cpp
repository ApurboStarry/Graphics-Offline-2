#include <bits/stdc++.h>

using namespace std;

struct Point {
  double x;
  double y;
  double z;
};

Point normalize(Point p) {
  double determinant = (p.x * p.x) + (p.y * p.y) + (p.z * p.z);
  determinant = sqrt(determinant);

  p.x /= determinant;
  p.y /= determinant;
  p.z /= determinant;

  return p;
}

int main() {
  Point l;
  l.x = 0;
  l.y = 0;
  l.z = -50;

  normalize(l);
}