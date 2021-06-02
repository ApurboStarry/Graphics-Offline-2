#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

#define PI 2*acos(0.0)

struct Point {
  double x;
  double y;
  double z;
};

Point eye;
Point look;
Point up;

double fov;
double aspectRatio;
double near;
double far;

vector<vector<double>> getIdentityMatrix4By4() {
  vector<vector<double>> vec(4, vector<double> (4, 0));
  vec[0][0] = 1;
  vec[1][1] = 1;
  vec[2][2] = 1;
  vec[3][3] = 1;

  return vec;
}

stack<vector<vector<double>>> matrixStack;

void inputInitials() {
  cin >> eye.x >> eye.y >> eye.z;
  cin >> look.x >> look.y >> look.z;
  cin >> up.x >> up.y >> up.z;
  cin >> fov >> aspectRatio >> near >> far;
}

vector<vector<double>> get4By1MatrixFromPoint(Point p) {
  vector<vector<double>> mat(4, vector<double> (1, 1));
  mat[0][0] = p.x;
  mat[1][0] = p.y;
  mat[2][0] = p.z;

  return mat;
}

vector<vector<double>> multiplyMatrices(vector<vector<double>> matrix1, vector<vector<double>> matrix2) {
  int rowSize = matrix1.size();
  int columnSize = matrix2[0].size();

  vector<vector<double>> newMatrix(rowSize, vector<double>(columnSize, 0));

  for(int i = 0; i < rowSize; i++) {
    for(int j = 0; j < columnSize; j++) {
      double value = 0;

      for(int k = 0; k < matrix1[i].size(); k++) {
        value += matrix1[i][k] * matrix2[k][j];
      }

      newMatrix[i][j] = value;
    }
  }

  return newMatrix;
}

void processTriangle() {
  for(int i = 0; i < 3; i++) {
    Point trianglePoint;
    cin >> trianglePoint.x >> trianglePoint.y >> trianglePoint.z;

    vector<vector<double>> newMatrix = multiplyMatrices(matrixStack.top(), get4By1MatrixFromPoint(trianglePoint));

    cout << fixed << setprecision(7) << newMatrix[0][0] << " " << newMatrix[1][0] << " " << newMatrix[2][0] << endl;
  }
  cout << endl;
}

vector<vector<double>> getScalingMatrix(double sx, double sy, double sz) {
  vector<vector<double>> matrix(4, vector<double>(4, 0));
  matrix[0][0] = sx;
  matrix[1][1] = sy;
  matrix[2][2] = sz;
  matrix[3][3] = 1;

  return matrix;
}

vector<vector<double>> getTranslationMatrix(double tx, double ty, double tz) {
  vector<vector<double>> matrix(4, vector<double>(4, 0));

  matrix[0][0] = 1;
  matrix[1][1] = 1;
  matrix[2][2] = 1;
  matrix[3][3] = 1;

  matrix[0][3] = tx;
  matrix[1][3] = ty;
  matrix[2][3] = tz;

  return matrix;
}

void printMatrix(vector<vector<double>> mat) {
  cout << "The Matrix" << endl;
  for(int i = 0; i < mat.size(); i++) {
    for(int j = 0; j < mat[i].size(); j++) {
      cout << mat[i][j] << " ";
    }
    cout << endl;
  }
  cout << "XXXXXXXXX" << endl;
}

Point getUnitVectorOfX() {
  Point i;
  i.x = 1;
  i.y = 0;
  i.z = 0;

  return i;
}

Point getUnitVectorOfY() {
  Point j;
  j.x = 0;
  j.y = 1;
  j.z = 0;

  return j;
}

Point getUnitVectorOfZ() {
  Point k;
  k.x = 0;
  k.y = 0;
  k.z = 1;

  return k;
}

Point scalerProductOfPoint(Point x, double scaler) {
  x.x *= scaler;
  x.y *= scaler;
  x.z *= scaler;

  return x;
}

double dotProductOfPoints(Point a, Point b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point crossProductOfPoints(Point a, Point b) {
  Point c;
  c.x = a.y * b.z - a.z * b.y;
  c.y = a.z * b.x - a.x * b.z;
  c.z = a.x * b.y - a.y * b.x;

  return c;
}

Point rotate(Point x, Point a, double theta) {
  Point result;
  Point temp1 = scalerProductOfPoint(x, cos(theta));
  Point temp2 = scalerProductOfPoint(a, (1-cos(theta)) * dotProductOfPoints(a, x));
  Point temp3 = scalerProductOfPoint(crossProductOfPoints(a, x), sin(theta));

  result.x = temp1.x + temp2.x + temp3.x;
  result.y = temp1.y + temp2.y + temp3.y;
  result.z = temp1.z + temp2.z + temp3.z;

  return result;
}

vector<vector<double>> getRotationMatrix(Point c1, Point c2, Point c3) {
  vector<vector<double>> mat(4, vector<double>(4, 0));
  
  mat[0][0] = c1.x;
  mat[1][0] = c1.y;
  mat[2][0] = c1.z;

  mat[0][1] = c2.x;
  mat[1][1] = c2.y;
  mat[2][1] = c2.z;

  mat[0][2] = c3.x;
  mat[1][2] = c3.y;
  mat[2][2] = c3.z;

  mat[3][3] = 1;

  return mat;
}

void performRotation() {
  double angle;
  Point a;
  cin >> angle >> a.x >> a.y >> a.z;

  angle = PI * angle / 180;

  Point c1 = rotate(getUnitVectorOfX(), a, angle);
  Point c2 = rotate(getUnitVectorOfY(), a, angle);
  Point c3 = rotate(getUnitVectorOfZ(), a, angle);

  vector<vector<double>> newMatrix = multiplyMatrices(matrixStack.top(), getRotationMatrix(c1, c2, c3));

  matrixStack.pop();
  matrixStack.push(newMatrix);
}

void processCommands() {
  matrixStack.push(getIdentityMatrix4By4());
  while(true) {
    string command;
    cin >> command;

    if(command == "triangle") {
      processTriangle();
    } else if(command == "translate") {
      double tx, ty, tz;
      cin >> tx >> ty >> tz;

      vector<vector<double>> newMatrix = multiplyMatrices(matrixStack.top(), getTranslationMatrix(tx, ty, tz));
      
      matrixStack.pop();
      matrixStack.push(newMatrix);
    } else if(command == "scale") {
      double sx, sy, sz;
      cin >> sx >> sy >> sz;

      vector<vector<double>> newMatrix = multiplyMatrices(matrixStack.top(), getScalingMatrix(sx, sy, sz));

      matrixStack.pop();
      matrixStack.push(newMatrix);
    } else if(command == "rotate") {
      performRotation();
    } else if(command == "push") {
      vector<vector<double>> matrixToPush = matrixStack.top();
      matrixStack.push(matrixToPush);
    } else if(command == "pop") {
      matrixStack.pop();
      
      vector<vector<double>> matrixToPush = matrixStack.top();
      matrixStack.push(matrixToPush);
    } else {
      break;
    }
  }
}

void stage1() {
  freopen("scene.txt", "r", stdin);
  freopen("stage1.txt", "w", stdout);

  inputInitials();
  processCommands();

  fclose(stdin);
  fclose(stdout);
}

void stage2() {

}

void stage3() {

}

void stage4() {

}

int main() {
  stage1();
  stage2();
  stage3();
  stage4();
}