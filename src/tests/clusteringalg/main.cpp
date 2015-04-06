
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>   
#include <iostream>
#include <vector>

using namespace cv;

int main( int argc, char** argv )
{

  Mat src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  std::vector<cv::Point> points;
  
  for( int y = 0; y < src.rows; y++ )
    for( int x = 0; x < src.cols; x++ )
      if(src.at<char>(x,y) >= 100)
        points.push_back(Point(y,x));


  imshow( "clustered image", src);
  waitKey( 0 );
}
