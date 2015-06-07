#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "opencvwidget.h"
#include "mycamerawindow.h"

int main(int argc, char **argv) {


    CvCapture * camera = cvCaptureFromCAM( 0 );

    QApplication app(argc, argv);
    MyCameraWindow *mainWin = new MyCameraWindow(camera);
    mainWin->setWindowTitle("OpenVideoMagnifier");
    mainWin->showMaximized();
    int retval = app.exec();

    cvReleaseCapture(&camera);

    return retval;
}
