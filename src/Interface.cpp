#include "Interface.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
 
#include "ClusterizeImage.h"

Interface::Interface() : _invert(false), _balance(true), _zoom(1), _saturation(0)
{
	run();
}

Interface::~Interface()
{ }

cv::Mat Interface::invertFilter(const cv::Mat& src) const
{
	return cv::Scalar::all(255) - src;
}

cv::Mat Interface::zoomFilter(const cv::Mat& src, const unsigned int zoom) const
{
	unsigned int value = std::log(zoom)/std::log(2);
	cv::Mat res = src;
	for(unsigned int i = 0; i < value; ++i)
	{
		cv::pyrUp(res, res, cv::Size(res.cols*2, res.rows*2));
		res = res(cv::Rect(src.size().width/2,src.size().height/2,src.size().width, src.size().height));
	}
	return res;
}

cv::Mat Interface::balanceFilter(const cv::Mat& src, const std::array<float,3>& percent)
{
    assert(src.channels() == 3);
    
    cv::Mat out;
 
    std::vector<cv::Mat> tmpsplit;
    split(src,tmpsplit);
    for(int i=0;i<3;i++) {
				float half_percent = percent[i] / 200.0f;
        //find the low and high precentile values (based on the input percentile)
        cv::Mat flat;
        tmpsplit[i].reshape(1,1).copyTo(flat);
        cv::sort(flat,flat,CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        
        //saturate below the low percentile and above the high percentile
        tmpsplit[i].setTo(lowval,tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval,tmpsplit[i] > highval);
        
        //scale the channel
        normalize(tmpsplit[i],tmpsplit[i],0,255,cv::NORM_MINMAX);
    }
    merge(tmpsplit,out);
    return out;
}

cv::Mat Interface::kmeans(const cv::Mat& src, unsigned int nbClusters)
{
  srand (time(NULL));

  cv::Mat base = src;
  cv::Mat res(base.size(), CV_8UC3, cv::Scalar(0, 0, 0));
  std::vector<cv::Point> points;
 
  for( int y = 0; y < base.rows; y++ )
    for( int x = 0; x < base.cols; x++ )
      if(base.at<char>(x,y) >= 100)
        points.push_back(cv::Point(y,x));

	std::vector<unsigned int> labels;
	std::vector<cv::Point> centers;

	ClusterizeImage clusterIm;
	clusterIm.kmeans(points, nbClusters, labels, 20, centers, base);

	std::vector<cv::Scalar> colors;
	for(unsigned int i = 0; i < nbClusters; ++i)
	{
		colors.push_back(cv::Scalar(rand()%255+1, rand()%255+1, rand()%255+1));
		circle(res, centers[i], 5, colors.at(i), 1);
	}

	for(unsigned int i = 0; i < points.size(); ++i)
		circle(res, points[i], 3, colors.at(labels[i]));

  cv::imwrite("out.png", res);
  return res;
}

cv::Mat Interface::saturate(const cv::Mat& src, const int saturateValue)
{
	cv::Mat out;
	cv::cvtColor(src, out, CV_BGR2HSV);

	for(int i = 0; i < out.rows ; i++)
		for(int j = 0; j < out.cols; j++)
			out.at<cv::Vec3b>(i,j)[1] += saturateValue;

	cv::cvtColor(out, out, CV_HSV2BGR);

	return out;
}

void Interface::run()
{
	cv::VideoCapture cap(0);

	assert(cap.isOpened());

	cvNamedWindow("Camera", CV_WINDOW_NORMAL) ;
	//cvSetWindowProperty("Camera", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	bool close = false;
	while(!close)
	{
		cv::Mat frame;

		bool bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			std::cout << "Cannot read the frame from camera" << std::endl;
			break;
		}

		std::array<float, 3> percent{ {1, 1, 1} };
		if(_balance)
			frame = balanceFilter(frame, percent);
		if(_invert)
			frame = invertFilter(frame);
		frame = saturate(frame, _saturation);
		frame = zoomFilter(frame,_zoom);
		
		cv::imshow("Camera", frame);

		//Control
		char c = cv::waitKey(30);
		if(c == 'i')
			_invert = !_invert;
		if(c == 'b')
			_balance = !_balance;
		if(c == 'p')
			_zoom*=2;
		if(c == 's')
			_saturation += 1;
		if(c == 'q')
			_saturation -= 1;
		if(c == 'z')
			_saturation = 0;
		if(c == 'm' && _zoom > 1)
			_zoom/=2;
		if(c == 27)
			close = true;
	}
}
