
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>   
#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <array>

void kmeansPerso(const std::vector<cv::Point>& points, unsigned int nbCluster,
						std::vector<unsigned int>& labels, unsigned int iter,
						std::vector<cv::Point>& centers, const cv::Mat& img)
{
	//Init random centers
	for(unsigned int i = 0; i < nbCluster; ++i)
		centers.push_back(cv::Point(rand()%img.cols+1, rand()%img.rows+1));
		
	int labelised[nbCluster];
	int meanHeight[nbCluster];
	int meanWidth[nbCluster];
	
	for(unsigned int it = 0; it < iter; ++it)
	{
		//Clean
		for(unsigned int i = 0; i < nbCluster; ++i)
		{
			labelised[i] = 0;
			meanHeight[i] = 0;
			meanWidth[i] = 0;
		}
		labels.clear();
		
		//for each point in points find the best center.
		for(unsigned int p = 0; p < points.size(); ++p)
		{
			int euclidianDist = std::sqrt(std::pow(centers[0].x-points[p].x,2)+std::pow(centers[0].y-points[p].y,2));
			int label = 0;
			for(unsigned int i = 1; i < centers.size(); ++i)
			{
				double tempDist = std::sqrt(std::pow(centers[i].x-points[p].x,2)+std::pow(centers[i].y-points[p].y,2));
				if(tempDist < euclidianDist)
				{
					euclidianDist = tempDist;
					label = i;
				}
			}
			labels.push_back(label);
			labelised[label] += 1;
			meanHeight[label] += points[p].y;
			meanWidth[label] += points[p].x;
		}
		
		//Move each center
		for(unsigned int i = 0; i < nbCluster; ++i)
		{
			centers[i] = cv::Point(meanWidth[i]/labelised[i], meanHeight[i]/labelised[i]);
		}
	}
}

int main( int argc, char** argv )
{
	srand (time(NULL));
	
  cv::Mat src = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat res(src.size(), CV_8UC3, cv::Scalar(0, 0, 0));
  std::vector<cv::Point> points;
  
  for( int y = 0; y < src.rows; y++ )
    for( int x = 0; x < src.cols; x++ )
      if(src.at<char>(x,y) >= 100)
        points.push_back(cv::Point(y,x));

	std::vector<unsigned int> labels;
	std::vector<cv::Point> centers;
	kmeansPerso(points, 3, labels, 20, centers, src);

	circle(res, centers[0], 5, cv::Scalar(255,0,0),1);
	circle(res, centers[1], 5, cv::Scalar(0,255,0),1);
	circle(res, centers[2], 5, cv::Scalar(0,0,255),1);
	circle(res, centers[3], 5, cv::Scalar(255,255,255),1);
	
	for(unsigned int i = 0; i < points.size(); ++i)
	{
		if(labels[i] == 0)
			circle(res, points[i], 3, cv::Scalar(255,0,0));
		if(labels[i] == 1)
			circle(res, points[i], 3, cv::Scalar(0,255,0));
		if(labels[i] == 2)
			circle(res, points[i], 3, cv::Scalar(0,0,255));
		if(labels[i] == 3)
			circle(res, points[i], 3, cv::Scalar(255,255,255));
	}

  cv::imshow( "Base", src);
  cv::imshow( "Final", res);
  cv::waitKey(0);
}
