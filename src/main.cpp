#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "opencvwidget.h"
#include "mycamerawindow.h"
#include <QFile>

int main(int argc, char **argv) {
    //Capture the last camera
    int id = 0;
    while(id+1 > id)
    {
        QFile cam("/dev/video"+QString::number(id));
        if(!cam.exists())
            break;
        id += 1;
    }
    if(id == 0)
        return -1;
    CvCapture * camera = cvCaptureFromCAM(id-1);
    //Launch the GUI
    QApplication app(argc, argv);
    MyCameraWindow *mainWin = new MyCameraWindow(camera);
    mainWin->setWindowIcon(QIcon("rsc/icon.png"));
    mainWin->setWindowTitle("OpenVideoMagnifier");
    mainWin->showMaximized();
    int retval = app.exec();

    //Release the camera
    cvReleaseCapture(&camera);

    return retval;
}
