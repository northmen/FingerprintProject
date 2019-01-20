//#include <opencv2/opencv.hpp>
#include </usr/local/Cellar/opencv@2/2.4.13.7_2/include/opencv2/opencv.hpp>
//#include <eigen3/Eigen/Dense>
#include </usr/local/Cellar/eigen/3.3.7/include/eigen3/Eigen/Dense>
#include <iostream>
#include <cmath>

#define PI 3.1415926536
#define FI -15.0
#define EMPTY 0.99
//#define EMPTY 0.0001
//#define EMPTY NULL

using namespace std;
using namespace Eigen;
using namespace cv;

MatrixXd rotation(MatrixXd M,float theta) {
  //MatrixXd M1=M;
  MatrixXd M1 = MatrixXd::Ones(M.rows(),M.cols());
  float a = theta;
  float angle = a*PI/180;
  int n = M.rows();
  int m = M.cols();
  int center_x = n/2;
  int center_y = m/2;
  float scale = 1; // (cos(angle) + float(n)/float(m)*sin(angle));
  
  for(int x=0; x<n; x++){
    for(int y=0;y<m; y++){
      M1(x,y) = EMPTY;
    }
  }
  
  for(int x=0; x<n; x++){
    for(int y=0;y<m; y++){
      int xp = ((x - center_x) * cos(angle) - (y - center_y) * sin(angle)) * scale + center_x ;
      int yp = ((x - center_x) * sin(angle) + (y - center_y) * cos(angle)) * scale + center_y;
      if((xp > 0 && xp < n ) && (yp > 0 && yp < m)){
        //M(x,y) = M1(xp,yp); //FORWAR method
        M1(xp,yp) = M(x,y); //BACKWARD method
      }
    }
  }
  return M1;
}

//INTERPOLATION FUNCTION
MatrixXd check(MatrixXd A) {
  MatrixXd m=A;
  for(int i=1;i<A.rows()-1; i++){
    for(int j=1; j<A.cols()-1; j++){
      if (m(i,j) == EMPTY) {
        cout << "i= " << i << "  j= " << j << endl; //coordinates of empty pixel
        int k = 0; //count how many neibours are not empty 
        if (m(i+1,j)   != EMPTY) { k += 1; }
        if (m(i+1,j+1) != EMPTY) { k += 1; }
        if (m(i,j+1)   != EMPTY) { k += 1; }
        if (m(i-1,j+1) != EMPTY) { k += 1; }
        if (m(i-1,j)   != EMPTY) { k += 1; }
        if (m(i-1,j-1) != EMPTY) { k += 1; }
        if (m(i,j-1)   != EMPTY) { k += 1; }
        if (m(i+1,j-1) != EMPTY) { k += 1; }
        m(i,j) = (m(i+1,j) + m(i-1,j) + m(i,j+1) + m(i,j-1) + m(i+1,j+1) + m(i+1,j-1) + m(i-1,j+1) + m(i-1,j-1) - (8-k)*EMPTY)/k;
      }
    }
  }
  return m;
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
  MatrixXd M2 = rotation(M, FI);
  M2 = check(M2); //INTERPOLATION
  Mat image2 = cast_to_int(M2);

  imwrite("resh_blurr.png", image2); //write the image to a file as PNG
  namedWindow("reshaped image",WINDOW_NORMAL);
  imshow("reshaped image", image2);
  waitKey( 0 );
  destroyWindow("reshaped image");
  return 0;
}