#ifndef __INTERFACE_H__
#define __INTERFACE__H__


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

class ClusterizeImage {
public:
    ClusterizeImage();
    ~ClusterizeImage();

    void kmeans(const std::vector<cv::Point>& points, unsigned int nbCluster,
                            std::vector<unsigned int>& labels, unsigned int iter,
                            std::vector<cv::Point>& centers, const cv::Mat& img);
private:
};

#endif
