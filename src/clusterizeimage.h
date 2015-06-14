#ifndef __INTERFACE_H__
#define __INTERFACE__H__

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

/**
 * This class provides some algorithm to clusterize an image into
 * some groups
 */
class ClusterizeImage {
public:
    ClusterizeImage();
    ~ClusterizeImage();
    /**
     * kmeans algorithm
     * @param points: all points
     * @param nbCluster: the number of clusters
     * @param labels: the label of each center
     * @param iter: the number of iterations for the algorithm
     * @param centers: the center of each cluster
     * @param img: the image to modify
     */
    void kmeans(const std::vector<cv::Point>& points, unsigned int nbCluster,
                            std::vector<unsigned int>& labels, unsigned int iter,
                            std::vector<cv::Point>& centers, const cv::Mat& img);
private:
};

#endif
