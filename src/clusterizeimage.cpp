#include "clusterizeimage.h"

ClusterizeImage::ClusterizeImage()
{}

ClusterizeImage::~ClusterizeImage()
{}

/**
 * kmeans algorithm
 * @param points: all points
 * @param nbCluster: the number of clusters
 * @param labels: the label of each center
 * @param iter: the number of iterations for the algorithm
 * @param: centers: the center of each cluster
 * @param img: the image to modify
 */
void ClusterizeImage::kmeans(const std::vector<cv::Point>& points, unsigned int nbCluster,
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
            centers[i] = cv::Point(meanWidth[i]/labelised[i], meanHeight[i]/labelised[i]);
    }
}

