#include "mycamerawindow.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <fstream>
#include <string.h>
#include <QProcess>

MyCameraWindow::MyCameraWindow(CvCapture *cam, QWidget *parent) : QWidget(parent), _invert(false), _balance(true),
                                                                  _zoom(1), _saturation(0), _contrast(1.0), _rotate(0)
{
    //load the config
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
        std::getline(confFile,line);
        _rotate = std::atoi(line.c_str());
        confFile.close();
    }

    //initialize the OpenCVWidget
    camera = cam;
    cvwidget = new QOpenCVWidget(this);
    cvwidget->setFixedSize(this->width(), this->height());
    QObject::connect(cvwidget, SIGNAL(balanceSignal()), this, SLOT(balanceClicked()));
    QObject::connect(cvwidget, SIGNAL(invertSignal()), this, SLOT(invertClicked()));
    QObject::connect(cvwidget, SIGNAL(zoomMSignal()), this, SLOT(zoomMClicked()));
    QObject::connect(cvwidget, SIGNAL(zoomPSignal()), this, SLOT(zoomPClicked()));
    QObject::connect(cvwidget, SIGNAL(saturationMSignal()), this, SLOT(satureMClicked()));
    QObject::connect(cvwidget, SIGNAL(saturationPSignal()), this, SLOT(saturePClicked()));
    QObject::connect(cvwidget, SIGNAL(ocrSignal()), this, SLOT(saveCaptureClicked()));
    QObject::connect(cvwidget, SIGNAL(saveSignal()), this, SLOT(saveConfig()));

    //timer between each capture
    startTimer(100);

    this->setStyleSheet("background-color: #12122e;");
}

/**
 * refresh the QOpenCVWidget
 */
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
    frame = rotateFilter(frame);

    IplImage* image2;
    image2 = cvCreateImage(cvSize(frame.cols,frame.rows),8,3);
    IplImage ipltemp=frame;
    cvCopy(&ipltemp,image2);
    cvwidget->putImage(image2);
}

/**
 * if we resize the window
 */
void MyCameraWindow::resizeEvent(QResizeEvent*)
{
    cvwidget->setFixedSize(this->width(), this->height());
}

/**
 * If a key is pressed
 */
void MyCameraWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_I)
        invertClicked();
    if(event->key() == Qt::Key_B)
        balanceClicked();
    if(event->key() == Qt::Key_P)
        zoomPClicked();
    if(event->key() == Qt::Key_D)
        saturePClicked();
    if(event->key() == Qt::Key_Q)
        satureMClicked();
    if(event->key() == Qt::Key_Z)
        _saturation = 0;
    if(event->key() == Qt::Key_M)
        zoomMClicked();
    if(event->key() == Qt::Key_R)
        rotateInc();
    if(event->key() == Qt::Key_T)
        rotateDec();
    if(event->key() == Qt::Key_S)
        saveConfig();
}

/**
 * React to some signals from QOpenCVWidget
 */
void MyCameraWindow::invertClicked()
{
    _invert = !_invert;
}

void MyCameraWindow::zoomPClicked()
{
    _zoom *= 2;
}

void MyCameraWindow::zoomMClicked()
{
    if(_zoom > 1)
        _zoom /= 2;
}

void MyCameraWindow::balanceClicked()
{
    _balance = !_balance;
}

void MyCameraWindow::saturePClicked()
{
    _saturation += 1;
}

void MyCameraWindow::satureMClicked()
{
    _saturation -= 1;
}

/**
 * invert filter
 * src: the image to invert
 */
cv::Mat MyCameraWindow::invertFilter(const cv::Mat& src) const
{
    return cv::Scalar::all(255) - src;
}

/**
 * zoom filter
 * src: the image to zoom on
 * zoom: the value of the zoom
 */
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

/**
 * balance filter
 * src: the image to modify
 * percent: the balance of r,g,b values
 */
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

/**
 * use clusterizeimage kmeans algorithm
 * src: the image to modify
 * nbClusters: the number of clusters
 */
cv::Mat MyCameraWindow::kmeans(const cv::Mat& src, unsigned int nbClusters)
{
    srand (time(NULL));

    cv::Mat base = src;
    cv::Mat res(base.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    std::vector<cv::Point> points;
    
    //Get all points
    for( int y = 0; y < base.rows; y++ )
        for( int x = 0; x < base.cols; x++ )
            if(base.at<char>(x,y) >= 100)
                points.push_back(cv::Point(y,x));

    std::vector<unsigned int> labels;
    std::vector<cv::Point> centers;

    ClusterizeImage clusterIm;
    clusterIm.kmeans(points, nbClusters, labels, 20, centers, base);

    //create some colors
    std::vector<cv::Scalar> colors;
    for(unsigned int i = 0; i < nbClusters; ++i)
    {
        colors.push_back(cv::Scalar(rand()%255+1, rand()%255+1, rand()%255+1));
        circle(res, centers[i], 5, colors.at(i), 1);
    }
    
    //draw points
    for(unsigned int i = 0; i < points.size(); ++i)
        circle(res, points[i], 3, colors.at(labels[i]));
    //write the out imaeg
    cv::imwrite("out.png", res);
    return res;
}

/**
 * saturate filter
 * src: the image to modify
 * saturateValue: the saturation
 */
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

cv::Mat MyCameraWindow::contrastFilter(const cv::Mat& image)
{
    cv::Mat new_image = cv::Mat::zeros(image.size(),image.type());
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            for( int c = 0; c < 3; c++ )
                new_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(_contrast*(image.at<cv::Vec3b>(y,x)[c]));
    return new_image;
}

cv::Mat MyCameraWindow::rotateFilter(const cv::Mat& image)
{
    cv::Mat out;
    cv::Point2f pt(image.cols/2., image.rows/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, _rotate, 1.0);

    //if(_rotate == 0 || _rotate == 180)
      cv::warpAffine(image, out, r, cv::Size(image.cols, image.rows));
    //else
      //cv::warpAffine(image, out, r, cv::Size(image.rows, image.cols));
    return out;
}


void MyCameraWindow::rotateInc()
{
    _rotate += 90;
    _rotate %= 360;
}


void MyCameraWindow::rotateDec()
{
    _rotate -= 90;
    _rotate %= 360;
}


void MyCameraWindow::saveConfig()
{
    std::ofstream configFile;
    configFile.open ("ovm.conf");
    configFile << _invert << std::endl
               << _balance << std::endl
               << _zoom << std::endl
               << _saturation << std::endl
               << _rotate << std::endl;
    configFile.close();
}

void MyCameraWindow::saveCaptureClicked()
{
    IplImage *image=cvQueryFrame(camera);
    cv::Mat frame(image);
    frame = contrastFilter(frame);
    cv::imwrite("ocr.png", frame);
    //TODO: get text zone
    //TODO hiera zones
    //TODO correct ocr
    //TODO configure
    QProcess::execute("gocr ocr.png -o gocrOutput");
    QProcess::startDetached("espeak -f gocrOutput -vfr+f4");
}
