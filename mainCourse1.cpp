//#include <opencv2/opencv.hpp>
#include </usr/local/Cellar/opencv@2/2.4.13.7_2/include/opencv2/opencv.hpp>
//#include <eigen3/Eigen/Dense>
#include </usr/local/Cellar/eigen/3.3.7/include/eigen3/Eigen/Dense>
#include <iostream>
#include <cmath>

#define PI 3.1415926536
#define SPOT_Y 168
#define SPOT_X 122
#define SPEED_X 1.0
#define SPEED_Y 1.0
#define R 0.04714
#define RX 1.0
#define RY 0.5

using namespace std;
using namespace Eigen;
using namespace cv;

int findCenterX(MatrixXd M) {
  int MinX=M.rows();
  for(int i=0; i<M.rows(); i++){
    for(int j=0;j<M.cols(); j++){
      if (M(i,j)>0.9 && i<MinX) {
         MinX = i;
      }
    }
  }
  int MaxX=0;
  for(int i=M.rows()-1; i>0; i--){
    for(int j=M.cols()-1; j>0; j--){
      if (M(i,j)>0.9 && i>MaxX) {
         MaxX = i;
      }
    }
  }
  int Xcenter = (MaxX + MinX)*0.5;
  cout << "Max X = " << MaxX << "Min X = " << MinX << endl;
  return Xcenter;
}

float decrease(float r, float Rad, float porog) {
  if (r < Rad-porog) {
    return 1;
  } else {
    float x = (Rad-r);
    return 1-exp(-(pow(x*150.0, 2)));
    //return 1-exp(-(pow(x*250.0, 2)));
  }
  //return (1 + exp(r - Rad)); //0.5*exp(-r)+0.5*exp(-10*r);
}

float c(int j, int i, MatrixXd M) {
  int X = M.rows();
  int Y = M.cols();
  float r2 = (RX*(i - SPOT_X)*(i - SPOT_X) + RY*(j - SPOT_Y)*(j - SPOT_Y)) / (X*X + Y*Y);
  float add = M(j,i);
  if (r2 < R) {
    return 1 - (1.0-add) * decrease(r2, R, R/3);
  } else {
    return 1.0;
  }
}

/*
float lagrange_newton(float A[N], float X[N], float x) {
  float r = 0.0;
  for (int i=N; i>0; i--) {
    r = A[i-1] + r * (x - X[i-1]);
  }
  return r;
}
*/

MatrixXd modification(MatrixXd M) {
  MatrixXd M1=M;
  int n = M.rows();
  int m = M.cols();
  int center_x = n/2;
  int center_y = m/2;
  float b = 0.5;
  for(int x=0; x<n; x++){
    for(int y=0;y<m; y++){
      //M1(x,y) = 2.0 - 2.0/(1.0 + M1(x,y)); //weak presure
      //M1(x,y) = pow(float(M1(x,y)),b);  // strong pressure
      M1(x,y) = pow(float(c(x,y,M1)),b);
    }
  }
  return M1;
}

MatrixXd cast_to_float(Mat A) {
  MatrixXd m(A.rows,A.cols);
  for(int i=0;i<A.rows; i++){
    for(int j=0; j<A.cols; j++){
      m(i,j) = (int)A.at<uchar>(i,j,0);
      m(i,j) = m(i,j)/255;
    }
  }
  return m;
}

Mat cast_to_int(MatrixXd m) {
  Mat A(m.rows(),m.cols(),CV_8UC1);
  for(int i=0;i<A.rows; i++){
    for(int j=0; j<A.cols; j++){
      int val = (int)(m(i,j)*255);
      A.at<uchar>(i,j,0) = (uchar)val;
    }
  }
  return A;
}

int main(int argc, char** argv) {
  Mat image=imread("clean_finger.png");
  MatrixXd M = cast_to_float(image);
  MatrixXd M2 = modification(M);
  //cout << findCenterX(M2) << endl;
  //cout << findCenterX(M2.transpose()) << endl;
  //int centerX = findCenterX(M2);
  //int centerY = findCenterX(M2.transpose());
  Mat image2 = cast_to_int(M2);
  //M2(centerX, centerY) = -10;
  cout << "m= " << M.rows() << "  n= " << M.cols() << endl; 
  imwrite("resh_blurr.png", image2); //write the image to a file as PNG
  namedWindow("reshaped image",WINDOW_NORMAL);
  imshow("reshaped image", image2);
  waitKey( 0 );
  destroyWindow("reshaped image");
  return 0;
}
