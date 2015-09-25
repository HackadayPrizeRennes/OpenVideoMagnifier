#ifndef MYCAMERAWINDOW_H_
#define MYCAMERAWINDOW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QtGui>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <math.h>
#include <array>

#include "opencvwidget.h"
#include "clusterizeimage.h"
#include "ocrthread.h"

/**
 * @brief The MyCameraWindow class
 * add some filters for the image
 */
class MyCameraWindow : public QWidget
{
    Q_OBJECT
    private:
        //The GUI
        QOpenCVWidget *cvwidget;
        //The camera
        CvCapture *camera;
        //Some filters
        bool _invert;
        bool _balance;
        unsigned int _zoom;
        int _saturation;
        double _contrast;
        int _rotate;
        bool _freeze;
        OCRThread _ocr;
        bool _fullscreen;

    public:
        MyCameraWindow(CvCapture *cam, QWidget *parent=0);
        ~MyCameraWindow();

        /**
         * invert the image
         * src: the image to invert
         */
        cv::Mat invertFilter(const cv::Mat& src) const;
        /**
         * zoom filter
         * src: the image to zoom on
         * zoom the value of the zoom
         */
        cv::Mat zoomFilter(const cv::Mat& src, const unsigned int zoom) const;
        /**
         * white balance
         * src: the img to balance
         * percent: the balance on the r,g,b values
         */
        cv::Mat balanceFilter(const cv::Mat& src, const std::array<float,3>& percent);
        /**
         * use clusterizeimage kmeans algorithm
         * src: the image to modify
         * nbClusters: the number of clusters
         */
        cv::Mat kmeans(const cv::Mat& src, unsigned int nbClusters);
        /**
         * saturate filter
         * src: the imagee to modify
         * saturateValue: the saturation
         */
        cv::Mat saturate(const cv::Mat& src, const int saturateValue);

        cv::Mat contrastFilter(const cv::Mat& image);

        cv::Mat rotateFilter(const cv::Mat& image);

        void fullscreen();

    public slots:
        //If the class receives some signals from QOpenCVWidget
        void invertClicked();
        void zoomPClicked();
        void zoomMClicked();
        void balanceClicked();
        void saturePClicked();
        void satureMClicked();
        void rotateInc();
        void rotateDec();
        void saveCaptureClicked();
        void saveConfig();

    protected:
        void timerEvent(QTimerEvent*);
        void resizeEvent(QResizeEvent*);
        void keyPressEvent(QKeyEvent *event);
};


#endif /*MYCAMERAWINDOW_H_*/
