#ifndef MYCAMERAWINDOW_H_
#define MYCAMERAWINDOW_H_

#include <QWidget>
#include <QVBoxLayout>
#include "opencvwidget.h"
#include "clusterizeimage.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <array>
#include <QtGui>

/**
 * @brief The MyCameraWindow class
 */
class MyCameraWindow : public QWidget
{
    Q_OBJECT
    private:
        QOpenCVWidget *cvwidget;
        CvCapture *camera;
        bool _invert;
        bool _balance;
        unsigned int _zoom;
        int _saturation;

    public:
        MyCameraWindow(CvCapture *cam, QWidget *parent=0);

        cv::Mat invertFilter(const cv::Mat& src) const;
        cv::Mat zoomFilter(const cv::Mat& src, const unsigned int zoom) const;
        cv::Mat balanceFilter(const cv::Mat& src, const std::array<float,3>& percent);
        cv::Mat kmeans(const cv::Mat& src, unsigned int nbClusters);
        cv::Mat saturate(const cv::Mat& src, const int saturateValue);

    public slots:
        void invertClicked();
        void zoomPClicked();
        void zoomMClicked();
        void balanceClicked();
        void saturePClicked();
        void satureMClicked();
        void saveConfig();

    protected:
        void timerEvent(QTimerEvent*);
        void resizeEvent(QResizeEvent*);
        void keyPressEvent(QKeyEvent *event);
};


#endif /*MYCAMERAWINDOW_H_*/
