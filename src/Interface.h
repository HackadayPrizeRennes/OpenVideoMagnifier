#ifndef __INTERFACE_H__
#define __INTERFACE__H__

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <array>

class Interface {
public:
	Interface();
	~Interface();

	cv::Mat invertFilter(const cv::Mat& src) const;
	cv::Mat zoomFilter(const cv::Mat& src, const unsigned int zoom) const;
	cv::Mat balanceFilter(const cv::Mat& src, const std::array<float,3>& percent);
	cv::Mat kmeans(const cv::Mat& src, unsigned int nbClusters);

	void run();
private:
	bool _invert;
	bool _balance;
	unsigned int _zoom;
};

#endif
