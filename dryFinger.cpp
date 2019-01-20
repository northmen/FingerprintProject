//#include <opencv2/opencv.hpp>
#include </usr/local/Cellar/opencv@2/2.4.13.7_2/include/opencv2/opencv.hpp>
//#include <eigen3/Eigen/Dense>
#include </usr/local/Cellar/eigen/3.3.7/include/eigen3/Eigen/Dense>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Eigen;
using namespace cv;

MatrixXd modification(MatrixXd M) {
  MatrixXd M1=M;
  int n = M.rows();
  int m = M.cols();
  float b = 0.5; // dry finger
  //float b = 2.0; // moist finger
  for(int x=0; x<n; x++){
    for(int y=0;y<m; y++){
      //M1(x,y) = 2.0 - 2.0/(1.0 + M1(x,y)); 
      M1(x,y) = pow(float(M(x,y)),b); 
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
  Mat image2 = cast_to_int(M2);
  cout << "m= " << M.rows() << "  n= " << M.cols() << endl; 
  imwrite("resh_blurr.png", image2); //write the image to a file as PNG
  namedWindow("reshaped image",WINDOW_NORMAL);
  imshow("reshaped image", image2);
  waitKey( 0 );
  destroyWindow("reshaped image");
  return 0;
}
