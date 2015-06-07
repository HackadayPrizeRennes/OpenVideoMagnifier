#include "mycamerawindow.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <fstream>
#include <string.h>

MyCameraWindow::MyCameraWindow(CvCapture *cam, QWidget *parent) : QWidget(parent), _invert(false), _balance(true), _zoom(1), _saturation(0) {
    std::ifstream confFile("ovm.conf");
    if(confFile.is_open())
    {
        std::string line;
        std::getline(confFile,line);
        _invert = std::atoi(line.c_str());
        std::getline(confFile,line);
        _balance = std::atoi(line.c_str());
        std::getline(confFile,line);
        _zoom = std::atoi(line.c_str());
        if(_zoom < 1) _zoom = 1;
        std::getline(confFile,line);
        _saturation = std::atoi(line.c_str());
        confFile.close();
    }

    camera = cam;
    cvwidget = new QOpenCVWidget(this);
    cvwidget->setFixedSize(this->width(), this->height());
    startTimer(100);
}

void MyCameraWindow::timerEvent(QTimerEvent*) {
    IplImage *image=cvQueryFrame(camera);
    cv::Mat frame(image);


    std::array<float, 3> percent{ {1, 1, 1} };
    if(_balance)
        frame = balanceFilter(frame, percent);
    if(_invert)
        frame = invertFilter(frame);
    frame = saturate(frame, _saturation);
    frame = zoomFilter(frame,_zoom);

    IplImage* image2;
    image2 = cvCreateImage(cvSize(frame.cols,frame.rows),8,3);
    IplImage ipltemp=frame;
    cvCopy(&ipltemp,image2);
    cvwidget->putImage(image2);
}


void MyCameraWindow::resizeEvent(QResizeEvent* event)
{
    cvwidget->setFixedSize(this->width(), this->height());
}

void MyCameraWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_I)
        _invert = !_invert;
    if(event->key() == Qt::Key_B)
        _balance = !_balance;
    if(event->key() == Qt::Key_P)
        _zoom*=2;
    if(event->key() == Qt::Key_D)
        _saturation += 1;
    if(event->key() == Qt::Key_Q)
        _saturation -= 1;
    if(event->key() == Qt::Key_Z)
        _saturation = 0;
    if(event->key() == Qt::Key_M && _zoom > 1)
        _zoom/=2;
    if(event->key() == Qt::Key_S)
        saveConfig();
}

cv::Mat MyCameraWindow::invertFilter(const cv::Mat& src) const
{
    return cv::Scalar::all(255) - src;
}

cv::Mat MyCameraWindow::zoomFilter(const cv::Mat& src, const unsigned int zoom) const
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

cv::Mat MyCameraWindow::balanceFilter(const cv::Mat& src, const std::array<float,3>& percent)
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


cv::Mat MyCameraWindow::kmeans(const cv::Mat& src, unsigned int nbClusters)
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

cv::Mat MyCameraWindow::saturate(const cv::Mat& src, const int saturateValue)
{
    cv::Mat out;
    cv::cvtColor(src, out, CV_BGR2HSV);

    for(int i = 0; i < out.rows ; i++)
        for(int j = 0; j < out.cols; j++)
            out.at<cv::Vec3b>(i,j)[1] += saturateValue;

    cv::cvtColor(out, out, CV_HSV2BGR);

    return out;
}


void MyCameraWindow::saveConfig()
{
    std::ofstream configFile;
    configFile.open ("ovm.conf");
    configFile << _invert << std::endl
               << _balance << std::endl
               << _zoom << std::endl
               << _saturation << std::endl;
    configFile.close();
}
